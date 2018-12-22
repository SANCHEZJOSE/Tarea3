#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"
#include "funciones.h"

#define BYTE unsigned char
#define TIME_OUT 200

BYTE macNodo[6];
BYTE macBROAD[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
Ethernet eBroad;
Protocolo pBroad;

int contProcesosAuto=0,reinicio=7;/*Contador de procesos automaticos 
y número máximo para reiniciar contadores de Broadcast*/
int ttl=5;

void procesoUsr(Matrices matriz);
void procesoAut(int *fn,int numPort, Matrices & info,char* Nombre,BYTE *macNodo);

int main(int argc, char* argv[]){
	Matrices InfoRed;
	strcpy((char *)eBroad.MACO,(char *)macNodo);
	strcpy((char *)eBroad.MACD,(char *)macBROAD);
	pBroad.cmd=1;
	pBroad.ttl=ttl;
	
	char namePort[11];
	int numPort = argc-2;
	getMacUsr(macNodo,argv[1]);
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
void procesoUsr(Matrices matriz){
	int opcion=0;
		char mensaje[10]="";
		printf("RaspiRed :\n");
		printf("A quien Desea Enviar un Mensaje?:\n");
		for (int i=0;i<8;i++){
			printf("%d.-\t",i+1);
			for (int j = 0; j <10; ++j){
				printf("%c",matriz.nombres[i][j]);
			}
			printf("\n");
		}
		printf("Ingrese Opcion:\t");
		opcion=fileno(stdin);
		printf("%d\n",fileno);
		fflush(stdin);
		printf("Ingrese Mensaje: \n");
		fflush(stdin);
		scanf("%c",&mensaje);
		fflush(stdin);
		//Ingresara Mensaje
}

void procesoAut(int *fn,int numPort, Matrices & info,char* Nombre,BYTE *macNodo){
	int tamMensaje=0;
	BYTE mensaje[126];
	for (int i = 0; i < numPort; ++i)
	{
		enviar(fn[i],(BYTE *)Nombre,strlen(Nombre),eBroad,pBroad);
	}
	for(int i=0;i<numPort;i++){
		Ethernet eaux;
		Protocolo paux;
		switch(tamMensaje=recibe(fn[i],i,mensaje,macNodo,200,eaux,paux)) {
		case 0://timeout
		break;
		case -1:{//Error
		printf("Error al recibir\n");
		fflush(stdout);
		}
		break;
		case -2:{//Enviar al destinatario
		paux.ttl--;//Se disminuye el ttl en 1
		//se vuelve a empaquetar para actualizar TTL
		empaquetarProtocolo(paux);
		empaquetarEthernet(paux,eaux);
		int nodo=existeMac(info,eaux.MACD);
		int puerto=mejorPuertoDestino(nodo,numPort,info);
		if ((nodo!=-1) && (puerto != -1) )//solo si el nodo esta registrado se reenviara(Evita inundación)
		{
			writeSlip(fn[puerto],eaux);
		}
		}
		break;
		case -3:{//Es broadcast
		paux.ttl--;//Se disminuye el ttl en 1
		//se vuelve a empaquetar para actualizar TTL
		empaquetarProtocolo(paux);
		empaquetarEthernet(paux,eaux);
		int nodo=gestionarNodo(i,info,eaux.MACO,paux.ttl,(char *)paux.frame);
		info.contBroadcast[nodo]++;
		if (paux.ttl>0)
		{
			for(int j=0;j<numPort;j++){
				if (j!=i)
				{
					writeSlip(fn[j],eaux);/*reenvia Broadcast por todos los
					 puertos menos por el puerto que se lee*/
				}
			}
		}
		}
		break;
		default:
		printf("Se recibio mensaje de %s :\n",NombreDeMac(info,eaux.MACO));
		mensaje[tamMensaje]='\0';
		printf("|%s|\n",(char *)mensaje);fflush(stdout);
		break;
		}
	}
	contProcesosAuto++;
	if (contProcesosAuto==reinicio)
	{
		for (int x = 0; x <NODOS; ++x)
		{
			if (info.contBroadcast[x]==0)
			{
				limpiarTTL(x,info);/*llena con -1 los TLL del nodo del que no se recibio Broadcast*/
			}
			else{
				info.contBroadcast[x]=0;//reinicia el contador de Broadcast de cada nodo
			}
		}

	}
}