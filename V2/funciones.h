#ifndef FUNCIONES_H
#define FUNCIONES_H
#define BYTE unsigned char
#include "funcionesStruct.h"
///Prototipos
//utilidad
int fcs(BYTE *x, int n);
int fcs (BYTE x);
//Enmascarado y enlace
void empaquetarProtocolo(Protocolo &);
bool desempaquetarProtocolo(Protocolo &);
int empaquetarEthernet(Protocolo &,Ethernet &);
bool desempaquetarEthernet(Protocolo &,Ethernet &);
void writeSlip(int,Ethernet& );
int readSlip(int,Ethernet& ,int);
void enviar(int fn,BYTE *mensaje, int largo, Ethernet &e,Protocolo &p);
int recibe(int fn,int puerto,BYTE * mensaje,BYTE * macNodo,int timeout_msec,Ethernet &e,Protocolo &p);
//Gestion de nodos
int gestionarNodo(int puerto,Matrices & info,BYTE * macOrigen,int TTL,char * nombre);
void getMacUsr(BYTE* mac,char *Nusr);
bool checkMac(BYTE *mac,BYTE *macusr);

//Tratamiento de matrices
int existeMac( Matrices & info,BYTE *mac);
int buscarEspacioNodo(Matrices & info);
void agregarNombre(int n,char *nombre,Matrices & info);
void agregarMac(int n,BYTE *macOrigen,Matrices & info);
void actualizarTTL(int n,int puerto,int TTL,Matrices & info);
char * NombreDeMac(BYTE * MAC);
void limpiarTTL(int nodo,Matrices & info);
void limpiarTTLs(Matrices & info);
void agregarmac(int n,BYTE *macOrigen,Matrices & info);
#endif
