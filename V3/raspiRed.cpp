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

int contProcesosAuto=0,reinicio=16;/*Contador de procesos automaticos 
y número máximo para reiniciar contadores de Broadcast*/
int ttl=8;
///variables para navegacion en el menú
int opcion1=0,opcion2=0,opcion3=0;
bool mostrarlista=true,mostrarpeticion=true;

void procesoUsr(int *fn,int numPorts, Matrices& matriz);
void procesoAut(int *fn,int numPort, Matrices & info,char* Nombre,BYTE *macNodo);

int main(int argc, char* argv[]){
	Matrices InfoRed;
	char namePort[11];//arreglo con identificadores de los puertos
	int numPort = argc-2;//numero de puertos
	getMacUsr(macNodo,argv[1]);//obtención de la mac del nodo a travez del nombre
	strcpy((char *)eBroad.MACO,(char *)macNodo);//configuraciones para paquete broadcast
	strcpy((char *)eBroad.MACD,(char *)macBROAD);
	pBroad.cmd=1;
	pBroad.ttl=ttl;
	empaquetarProtocolo(pBroad);
	empaquetarEthernet(pBroad,eBroad);
	int fn[numPort];
	for(int i=0;i<numPort;i++){
   		strcpy(namePort,argv[i+2]);
		fn[i] = openPort(namePort,B38400);
	}
	printf("Bienvenido usuaria(o) %s a la RaspiRed :\n--Presione Enter si desea enviar un mensaje.",argv[1]);
	while(true){
	procesoUsr(fn,numPort,InfoRed);
	procesoAut(fn,numPort,InfoRed,argv[1],macNodo);
	}
	return 0;
}
void procesoUsr(int *fn,int numPorts, Matrices& matriz){
	int in=fileno(stdin);
	BYTE mensaje[126];
	BYTE opcion[3];
	Protocolo paquete;
	Ethernet ethernet;
	if(opcion1==0)
		opcion1=readPort(in,opcion,1,TIME_OUT);//si dentro del time out no ingresa opcion el sistema vuelve a esperar
	if(opcion1>0){
		if (opcion2==0)
		{
			if(mostrarlista){//permite mostrar lista una vez
				fflush(stdin);
				printf("--A quien Desea Enviar un Mensaje?:\n");fflush(stdout);
				//Imprime lista de nombres
				for (int i=0;i<NODOS;i++){
					if( EstadoNodo(i,numPorts,matriz)){
						printf("%d.-\t%s\n",i+1,matriz.nombres[i]);fflush(stdout);
					}
				}
				printf( "Ingrese Opcion:\t");fflush(stdout);
				mostrarlista=false;
			}
			opcion2=readPort(in,opcion,2,TIME_OUT);//
			fflush(stdin);
			opcion[opcion2-1]='\0';
		}
		if (opcion2>0)
		{	

			if (opcion3==0)
			{
				int numero = atoi((char*)opcion)-1;//transforma el string a numero -1
				memset(opcion, 0 , sizeof(BYTE)*3);
				if ((numero<15 && numero>-1) && EstadoNodo(numero,numPorts,matriz))
				{
					fflush(stdin);
					if (mostrarpeticion)
					{
						printf("Ingrese Mensaje: ");fflush(stdout);
						mostrarpeticion=false;//permite motrar esto una vez
					}
					opcion3=readPort(in,mensaje,126,TIME_OUT);
					fflush(stdin);
					if (opcion3>0)
					{
						paquete.cmd=0;
						paquete.ttl=ttl;
						mensaje[opcion3-1]='\0';
						int puerto=mejorPuertoDestino(numero,numPorts,matriz);//se selecciona el mejor puerto
						strcpy( (char*)ethernet.MACO, (char*)macNodo);//se copia la mac del nodo en Ethernet(origen)
						strcpy( (char*)ethernet.MACD , (char*)matriz.mac[numero]);//se copia la mac de destino en Ethernet
						enviar(fn[puerto],mensaje,opcion3,ethernet,paquete);
						opcion1=0;//Se reinicia el proceso usuario
						opcion2=0;
						opcion3=0;
						memset(ethernet.frameEth, 0 , sizeof(BYTE)*strlen((char*)ethernet.frameEth));//limpia los frames
						memset(paquete.frame, 0 , sizeof(BYTE)*strlen((char*)paquete.frame));
						mostrarpeticion=true;
						mostrarlista=true;
						printf("--Presione Enter si desea enviar un mensaje.\n");fflush(stdout);

					}
				}
				else{
					printf("Ingrese una opcion valida.\n");
					opcion3=0;
					opcion2=0;
					mostrarlista=true;
					mostrarpeticion=true;
				}	
			}

		}
	}
}
void procesoAut(int *fn,int numPort, Matrices & info,char* Nombre,BYTE *macNodo){
	int tamMensaje=0;
	BYTE mensaje[126];
	Ethernet eaux;
	Protocolo paux;
	for (int i = 0; i < numPort; ++i)
	{
		enviar(fn[i],(BYTE *)Nombre,strlen(Nombre),eBroad,pBroad);
	}
	for(int i=0;i<numPort;i++){
		switch(tamMensaje=recibe(fn[i],i,mensaje,macNodo,TIME_OUT,eaux,paux)) {
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
		memset(eaux.frameEth, 0 , sizeof(BYTE)*strlen((char*)eaux.frameEth));//limpia los frames
		memset(paux.frame, 0 , sizeof(BYTE)*strlen((char*)paux.frame));
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
