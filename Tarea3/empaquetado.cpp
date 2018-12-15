#include "empaquetado.h"
#include "serial.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>


BYTE END = 0xC0;// 192
BYTE ESC = 0xDB;// 219
BYTE DC = 0xDC;// 220
BYTE DD = 0xDD;// 221

int fcs(BYTE *x, int n){
    int c=0;
    for(int j=0;j<n;j++)
        for(int i=0;i<8;i++)
           c+= (x[j] & (0x01 <<i)) >>i;
    return c;
}

int fcs(BYTE x) {
    int c = 0;
    for (int i = 0 ; i < 8 ; i++) {
        if ( (x >> i) & 0x01 ) {
            c++;
        }
    }
    return c;
}
void empaquetarProtocolo(Protocolo &p){
    if (p.Long > 0) {
        //3 bits CMD	+	4 bits ttl	+	1 bit long
        p.frame[0] = (p.cmd & 0x07) | ((p.ttl&0x0F)<<3) | ((p.Long &0x01)<<7);
        //6 bits Long	+	2 bits Data
        p.frame[1] = ((p.Long&0XFE)>>1 | ((p.data[0] & 0x03) << 6));
        for ( int i = 0 ; i < p.Long - 1; i++ ) {
            //6 bits        +       2 bits
            p.frame[2 + i] = ((p.data[i] & 0xFC) >> 2) | ((p.data[i + 1] & 0x03) << 6);
        }   
        p.frame[p.Long + 1] = ((p.data[p.Long - 1] & 0xFC) >> 2);
        p.fcs = fcs(p.frame,p.Long + 2);
        //calcular checksum
        p.frame[p.Long + 1] =((p.data[p.Long - 1] & 0xFC) >> 2) | ((p.fcs & 0x03) << 6);
        //agrega checksum a frame
        p.frame[p.Long + 2] = (p.fcs)>>2;
        }else {
	        p.frame[0] = (p.cmd & 0x07) | (p.ttl<<3);
	        p.fcs = fcs(p.frame,1);
	        p.frame[1] = (p.fcs & 0x03) << 6;
	        p.frame[2] = (p.fcs>>2);
        }
}
bool desempaquetarProtocolo(Protocolo &p){
    p.cmd = p.frame[0] & 0x07; //3 bits de comandos
    p.ttl = (p.frame[0] & 0x78)>>3;//4 bits ttl
    p.Long = ((p.frame[0] & 0x80)>>7) | ((p.frame[1] & 0x3F) << 1);//7 bits long
    p.fcs = fcs(p.frame, p.Long + 1) + fcs((p.frame[p.Long + 1] & 0x3F));

    printf("el checksum es %d\n",p.fcs);

    //obtención del checksum al desempaquetar
    int aux=((p.frame[p.Long + 1]>>6) | ((p.frame[p.Long + 2]) << 2));
    printf("este es el checksum %d\n",aux);
    if ( p.fcs ==(int)((p.frame[p.Long + 1] >>6) | ((p.frame[p.Long + 2]) <<2))) {
        printf("entre\n");
        //checksum obtenido vs checksum desempaquetado
        if (p.Long > 0){
            p.data[0] = (p.frame[1] >> 6) | ((p.frame[2] & 0x3F) << 2);
            //obtención de la data desde el frame
            for (int i = 0 ; i < p.Long; i++) {
                p.data[i + 1] = (p.frame[i + 2] >> 6) | ((p.frame[i + 3] & 0x3F) << 2);
            }
        }
    return true; //si checksum es correcto
    }
    return false;//si checksum es incorrecto
}
int empaquetarEthernet(Protocolo &p,Ethernet &e){
    e.Long=p.Long+3;//tamaño de data más los campos del protocolo
    for(int i=0;i<6;i++){
        e.frameEth[i]=e.MACD[i];//mac destino
        e.frameEth[i+6]=e.MACO[i];//mac origen
    }
    e.frameEth[12] = e.Long & 0xFF;
    e.frameEth[13] = (e.Long & 0xFF00)>>8;
      for(int i=0;i<e.Long;i++)
        e.frameEth[14+i]=p.frame[i];
    e.fcs = fcs(e.frameEth,14+e.Long);//tamaños de (macs+long+frame)
     for(int i=0;i<4;i++)
    e.frameEth[14+e.Long+i] = (e.fcs &(0xFF<<8*i)) >> 8*i;
return (14+e.Long+4);//tamaños de (macs+long+frame+fcs)
}

bool desempaquetarEthernet(Protocolo &p,Ethernet &e){
    e.Long = e.frameEth[12] +(e.frameEth[13] << 8);//obtencion del largo de la data desde
    // el frame de ethernet
    int FCS =fcs(e.frameEth,14+e.Long);//cálculo del fcs directamente del frame
    e.fcs= 0;
    for(int i=0;i<4;i++)
        e.fcs = e.fcs + (e.frameEth[14+e.Long+i] << (8*i));//desempaquetado de fcs ethernet
    if(FCS == e.fcs){//detección de error por checksum
        for (int i = 0; i <6; ++i)
        {
        e.MACD[i]=e.frameEth[i];//mac destino
        e.MACO[i]=e.frameEth[i+6];//mac origen
        //printf(" %x ",e.MACD[i]);fflush(stdout);
        //printf(" %x ",e.MACO[i]);fflush(stdout);
        }
        for (int i = 0; i <e.Long; ++i)
        {
            p.frame[i]=e.frameEth[14+i];
        }
        return true;//Comprobación de error FCS correcta (en Ethernet)
    }else{
        return false;//Comprobación de error FCS invalida (en Ethernet)
    }
}

void writeSlip(int fn,Ethernet& ether){
    writePort(fn,(&END),1);//Caracter de inicio
    for(int i=0;i<(ether.Long+18);i++){
        if(ether.frameEth[i]==END){
            writePort(fn,&ESC,1);
            writePort(fn,&DC,1);
        }else
            if(ether.frameEth[i]==ESC){
                writePort(fn,&ESC,1);
                writePort(fn,&DD,1);
            }else
                writePort(fn,&ether.frameEth[i],1);
    }
    writePort(fn,(&END),1);//Caracter de termino
}
void readSlip(int fn,Ethernet& ether,int timeout_msec){
        int n,i=0;
        BYTE c;
        do {
            n=readPort(fn,&c,1,timeout_msec);
        } while(n==0 || c!=END);

        do {
            n=readPort(fn,&c,1,timeout_msec);
            if(c==ESC) {
                n=readPort(fn,&c,1,timeout_msec);
                if(c==DC)
                    ether.frameEth[i]=END;
                if(c==DD)
                    ether.frameEth[i]=ESC;
                i++;    
            } else if(c!=END) {
                ether.frameEth[i]=c;
                i++;
            }
        }while(c!=END);
}