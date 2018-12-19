#ifndef FUNCIONES_H
#define FUNCIONES_H
#define BYTE unsigned char
#include "funcionesStruct.h"
///Prototipos
void empaquetarProtocolo(Protocolo &);
bool desempaquetarProtocolo(Protocolo &);
int empaquetarEthernet(Protocolo &,Ethernet &);
bool desempaquetarEthernet(Protocolo &,Ethernet &);
void writeSlip(int,Ethernet& );
int readSlip(int,Ethernet& ,int);
int fcs(BYTE *x, int n);
int fcs (BYTE x);

void enviar(int fn,BYTE *mensaje, int largo, Ethernet &e,Protocolo &p);
int recibe(int fn,BYTE * mensaje,int timeout_msec,Ethernet &e,Protocolo &p);
void getMacUsr(BYTE* mac,char *Nusr);


//Tratamiento de matrices
int existeMac( unsigned char mac[][6],unsigned char m2[],int N );
unsigned char agregarMac(unsigned char m[][6], unsigned char m2[], int pos);
void agregarNombre(char* nombres,char* newName,int pos);
int ActualizarTtl(int* ttl[][4],int newTtl,int nodo,int puerto);
#endif
