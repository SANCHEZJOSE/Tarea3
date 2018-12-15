#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"
#include "funciones.h"
#define BYTE unsigned char
#define TIME_OUT 200

int main(int argc, char* argv[]){
	//-------------------------------------+
	//sudo ./anillo |juan | roberto | alex |
	//-------------------------------------+
	//              | pt1 |   pt2   | pt3  |
	//-------------------------------------+

	//-------------------------//Abrir Puertos
	Protocolo paquete;
	Ethernet ethernet;
	Slip slip;
	int N=argc-2;//Numero de puertos
	for(int i=0;I<N;i++)
		fn[i]=openPort(argv[i+1],89600);
	int in =fileno(stdin);
	
	while(true){
		//casos
		switch(paquete.ttl){
			case '1':
				break;
			case '2':
				break;
			case '3':
				break;
			case '4':
				break;
		}
		//-------------------------//Broadcast
		switch()
		for(int i=0;i<N;i++){
			x=readPort(fn[i],data,20,1000);
			if(x>0){
				printf("Mensaje Recibido|%s|\n",(char*)data);
				for(int j=0;j<N;j++){//reenvio
					writePort(fn[j],data,x);
					printf("Mensaje Enviado |%s|\n",(char*)data);
				}
			}
		}
		//-------------------------//
	}
	return 0;
}
