#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"
#include "funciones.h"
#define BYTE unsigned char
#define TIME_OUT 200
void procesoUsr()
void procesoAut()
int main(int argc, char* argv[]){
	BYTE macNodo[6];
	getMacUsr(macNodo,argv[1])
	Matrices InfoRed;
	char namePort[11];
	int numPort = argc-2;
	char aux[4];
	strcpy(aux, argv[1]);
	int dir = (int)aux[0]-'0';
	int fn[numPort];
	for(int i=0;i<numPort;i++){
   		strcpy(namePort,argv[i+2]);
		fn[i] = openPort(namePort,B38400);
	}
	int in=fileno(stdin);
	return 0;
}
void procesoUsr(){}
void procesoAut(int *fn,int numPort, Matrices & info){
	Ethernet eaux;
	Protocolo paux;
	BYTE mensaje[126];
	for(int i=0;i<numPort;i++){
		switch(recibe(fn[i],mensaje,200,eaux,paux)) {
			case 0://timeout
			break;
			case -1://Error
			printf("Error al recibir\n");
			fflush(stdout);
			break;
			case -2://Enviar al destinatario
			break;
			case -3://Es broadcast
			break;
			default:
			printf("Se recibio mensaje de %s\n",NombreDeMac(eaux.MACO));
			break;
	}
	//enviarbroadcast();
}
}
