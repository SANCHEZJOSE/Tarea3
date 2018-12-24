#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"
#include "funciones.h"

#define BYTE unsigned char
#define TIME_OUT 10

BYTE macNodo[6];
BYTE macBROAD[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


int contProcesosAuto=0,reinicio=600;/*Contador de procesos automaticos 
y número máximo para reiniciar contadores de Broadcast*/
int ttl=8;
///variables para navegacion en el menú
int opcion1=0,opcion2=0,opcion3=0;
bool mostrarlista=true,mostrarpeticion=true;

void procesoUsr(int *fn,int numPorts, Matrices& matriz);
void procesoAut(int *,int , Matrices & ,char*,BYTE *);


int main(int argc, char* argv[]){

	Matrices InfoRed;
	//char namePort[11];//arreglo con identificadores de los puertos
	int numPort = argc-2;//numero de puertos
	getMacUsr(macNodo,argv[1]);//obtención de la mac del nodo a travez del nombre
	int fn[numPort];
	for(int i=0;i<numPort;i++){
   		//strcpy(namePort,argv[i+2]);
		fn[i] = openPort(argv[i+2],B38400);
	}
	printf("Bienvenid@ usuari@ %s a la RaspiRed :\n--Presione Enter si desea enviar un mensaje.\n",argv[1]);
	fflush(stdout);
	while(true){
	procesoUsr(fn,numPort,InfoRed);
	procesoAut(fn,numPort,InfoRed,argv[1],macNodo);
// for (int i = 0; i < 15; ++i){
// 		for (int j = 0; j <4; j++)
// 			printf("|\t%d\t|",InfoRed.ttl[i][j]);
// 		printf("\n");
// 	}
// 	printf("\n");
// 	for (int i = 0; i <15 ; ++i){
// 		for (int j = 0; j < 6 ; j++)
// 			printf("|\t%x\t|",InfoRed.mac[i][j]);
// 		printf("\n");
// 	}
// 	printf("\n");
// 	for (int i = 0; i <15 ; ++i){
// 		printf("|");
// 		for (int j = 0; j < 10 ; j++)
// 			printf("%c",InfoRed.nombres[i][j]);
// 		printf("|");
// 		printf("\n");
// 	}
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
				printf("--A quien Desea Enviar un Mensaje?:\n");
				//Imprime lista de nombres
				for (int i=0;i<NODOS;i++){
					if( EstadoNodo(i,numPorts,matriz)){
						printf("%d.-\t%s\n",i+1,matriz.nombres[i]);fflush(stdout);
					}
				}
				printf( "Ingrese Opcion:\t");fflush(stdout);
				mostrarlista=false;
			}
			opcion2=readPort(in,opcion,3,TIME_OUT);//
			fflush(stdin);
			opcion[opcion2-1]='\0';
		}
		if (opcion2>0)
		{	

			if (opcion3==0)
			{
				int numero = atoi((char*)opcion)-1;//transforma el string a numero -1
				memset(opcion, 0 , sizeof(BYTE)*3);
				bool estadonodo=false;
				if ((numero<15 && numero>-1))
				{
					printf("%d\n",numero);fflush(stdout);
					estadonodo=EstadoNodo(numero,numPorts,matriz);
				}
				if (estadonodo )
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
						paquete.cmd=1;
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
	if (contProcesosAuto==10)/*solo cuando el contador de proceso 
	es 30 se envia un broadcast(evita inundacion)*/
	{
		Ethernet eBroad;
		Protocolo pBroad;
		for (int m = 0; m < 6; ++m)
		{
			eBroad.MACO[m]=macNodo[m];
			eBroad.MACD[m]=macBROAD[m];
		}
		pBroad.cmd=0;
		pBroad.ttl=ttl;
		pBroad.Long=strlen(Nombre);
		strcpy((char*)pBroad.data,Nombre);
		empaquetarProtocolo(pBroad);
		empaquetarEthernet(pBroad,eBroad);

		{
				for (int i = 0; i < numPort; ++i)
			{

			writeSlip(fn[i],eBroad);

			}
		}
	}


	for(int i=0;i<numPort;i++){
		tamMensaje=recibe(fn[i],i,mensaje,macNodo,TIME_OUT,eaux,paux);
		switch(tamMensaje) {
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
		paux.data[paux.Long]='\0';
		// 	printf("|%d|%d|%d|%s|puerto|%d|\t Ether|",paux.cmd,paux.ttl,paux.Long,(char*)paux.data,i);
		// 	for (int impmac = 0; impmac < 6; ++impmac)
		// {
		// 	printf("%x",eaux.MACD[impmac]);
			
		// }
		// printf("|");
		// 	for (int impmac = 0; impmac < 6; ++impmac)
		// {
		// 	printf("%x",eaux.MACO[impmac]);
			
		// }
		// printf("|\n");
		fflush(stdout);
		paux.ttl--;//Se disminuye el ttl en 1
		//se vuelve a empaquetar para actualizar TTL
		
		empaquetarProtocolo(paux);
		empaquetarEthernet(paux,eaux);
		int nodo=gestionarNodo(i,info,eaux.MACO,paux.ttl,(char *)paux.data);
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
		memset(eaux.frameEth, 0 ,129);//limpia los frames
		memset(paux.frame, 0 ,148);
	}

	contProcesosAuto++;
	if (contProcesosAuto==reinicio)
	{
		for (int x = 0; x <NODOS; ++x)
		{
			if (info.contBroadcast[x]==0)
			{
				limpiarTTL(x,numPort,info);/*llena con -1 los TLL del nodo del que no se recibio Broadcast*/
			}
			else{
				info.contBroadcast[x]=0;//reinicia el contador de Broadcast de cada nodo
			}
		}
		contProcesosAuto=0;

	}
}
