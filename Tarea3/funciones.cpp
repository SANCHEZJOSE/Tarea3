#include "funciones.h"
#include "sincrona.h"
#include <string.h>
#include <stdlib.h>

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
void enviar(BYTE *frame, int largo,int destino, int origen){

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

    writePort(,size);
}

int recibe(BYTE * mensaje,int timeout_msec,int origen){
    Ethernet e;
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
    }
}
//-------------------------//

//-------------------------//
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

//Tratamiento de MAC
int existeMac( unsigned char mac[nodos][6] ,unsigned char m2[6],int N ){//pasar de in a byte
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
//Tratamiento de Nombres
void agregarNombre(char nombres[nodos][10],char newName[10],int pos){
	for(int =0;i<10,i++)
		nombres[pos+1]=newName[i];
}
//Tratamiento ttl
void ActualizarTtl(int ttl[nodos][3],int newTtl,int nodo,int puerto){
	ttl[nodo][puerto]=newTtl;
}















