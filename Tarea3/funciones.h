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
void readSlip(int,Ethernet& ,int);
int fcs(BYTE *x, int n);
int fcs (BYTE x);

void enviar(int fn,BYTE *mensaje, int largo, Ethernet &e,Protocolo &p);
int recibe(int fn,BYTE * mensaje,int timeout_msec,Ethernet &e,Protocolo &p);
void getMacUsr(BYTE* mac,char *Nusr);
bool checkMac(BYTE *mac,int nodo);
//Tratamiento de matrices
int existeMac( int* m,int* m2,int N );
void agregarMac(int* m, int* m2, int pos);
void agregarNombre(char* nombres,char* newName,int pos);
void ActualizarTtl(int* ttl,int newTtl,int nodo,int puerto);
#endif
