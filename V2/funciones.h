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

void gestionarNodo(int puerto,Matrices & info,BYTE * macOrigen,int TTL,char * nombre);
void enviar(int fn,BYTE *mensaje, int largo, Ethernet &e,Protocolo &p);
int recibe(int fn,BYTE * mensaje,int timeout_msec,Ethernet &e,Protocolo &p);
void getMacUsr(BYTE* mac,char *Nusr);
bool checkMac(BYTE *mac,BYTE *macusr);
void agregarmac(int n,BYTE *macOrigen,Matrices & info);
//Tratamiento de matrices
int existeMac( Matrices & info,BYTE *mac);
int buscarEspacioNodo(Matrices & info);
void agregarNombre(int n,char *nombre,Matrices & info);
void agregarmac(int n,BYTE *macOrigen,Matrices & info);
void actualizarTTL(int n,int puerto,int TTL,Matrices & info);
char * NombreDeMac(BYTE * MAC);
#endif
