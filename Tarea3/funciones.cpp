
#include "funciones.h"
#include "serial.h"
#include "funcionesStruct.h"
#include <string.h>
#include <stdio.h>
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
//-------------------------//MODIFICAR
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
void enviar(BYTE *frame, int largo,int destino, int origen){
/*
    Protocolo paquete;
    Ethernet ethernet;

    getMac(ethernet.MACD,destino);
    getMac(ethernet.MACO,origen);

    paquete.cmd=3;
    paquete.Long=largo;

    for(int i=0;i<largo;i++)
        paquete.data[i]=frame[i];
    empaquetarProtocolo(paquete);
    int size=empaquetarEthernet(paquete,ethernet);
    empaquetarSlip(Ethernet);
    //writePort(,size);
    */
    return 0;
}
int recibe(BYTE * mensaje,int timeout_msec,int origen){
  	/*Ethernet e;
    Protocolo p;
    int size=readPort(e.ethernet,timeout_msec);
    if (size>0){
    if (desempaquetarEthernet(p,e)){//Revision del FCS(Ethernet)
        if(checkMac(e.MACD,origen)){//Verificar correspondencia de MAC
            if(desempaquetarProtocolo(p)){//Revision del FCS(Protocolo)
                int n=p.Long;
                strcpy ( (char *)mensaje,(char *)p.data);
                return n;
            }
            else{
                return -1;//Comprobacion de error FCS invalida (en protocolo)!
            }
        }
        else{
            writePort(e.ethernet,14+e.Long+4);
            return -2;//el nodo no es el destinatario por lo tanto se reenvia el paquete
            }  
    }
    else{
        return -1;//Comprobacion de error FCS invalida (en Ethernet)!
    }
    }
    else{
        return 0;//timeout
    }*/
    return 0;
}
void getMac(BYTE* mac, int n){
    char macAux[18];
    FILE * arch = fopen("macs.txt","r");
    fseek(arch, 23*n,SEEK_SET); //58-23
    fseek(arch,5,SEEK_CUR); //38-5
    fscanf(arch,"%s",macAux);
    //printf("%s\n",macAux);
    //fflush(stdout);
    for(int i=0;i<6;i++){//ya que son 6 BYTE
        for(int j=0;j<2;j++){//ya que son dos cuaternas
            char aux=macAux[i*3+j];
            int cuaterna=0;
            if(aux>='0' && aux<='9')//es digito!
                cuaterna=aux-'0';
            if(aux>='a' && aux<='z')//es minuscula!
                cuaterna=aux-'a'+10;
            if(aux>='A' && aux<='Z')//es mayuscula!
                cuaterna=aux-'A'+10;
            if(j==0)
                mac[i] = cuaterna<<4;
            if(j==1)
                mac[i]=mac[i] | cuaterna;
        }
    }
}
bool checkMac(BYTE *mac,int nodo){
    BYTE aux[6];
    getMac(aux,nodo);//mac del nodo
    return strncmp((char *)mac,(char *)aux,6)==0;//true si la mac es del nodo
    //false si no corresponde.   
}
int existeMac( unsigned char mac[][] ,unsigned char m2[],int N ){//pasar de in a byte
	bool aux=true;
	for(int i=0;i<N;i++){//cantidad de mac
		aux=true;
		for(int j=0;j<6;j++){
				aux=aux & (mac[i][j]==m2[j]) ;
		}
		if(aux)
			return i;
	}
	return -1;
}
void agregarMac(unsigned char m[nodos][6], unsigned char m2[6], int pos){
	if(existeMac(m[nodos][6],m2[6],pos)==1){
		for(int i=0;i<6;i++)
			m[pos+1][i]=m2[i];
	}else{
		printf("La Mac ya Existe\n");
	}
}
void agregarNombre(char nombres[nodos][10],char newName[10],int pos){
	for(int =0;i<10,i++)
		nombres[pos+1]=newName[i];
}
void ActualizarTtl(int ttl[nodos][3],int newTtl,int nodo,int puerto){
	ttl[nodo][puerto]=newTtl;
}















