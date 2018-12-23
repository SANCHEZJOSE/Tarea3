#ifndef FUNCIONES_H
#define FUNCIONES_H
#define BYTE unsigned char
#include "funcionesStruct.h"
///Prototipos
//utilidad
int fcs(BYTE *x, int n);
int fcs (BYTE x);
//Enmascarado y enlace
void empaquetarProtocolo(Protocolo &p);
bool desempaquetarProtocolo(Protocolo &p);
int empaquetarEthernet(Protocolo &p,Ethernet &e);
bool desempaquetarEthernet(Protocolo &p,Ethernet &e);
void writeSlip(int fn,Ethernet & ether);
int readSlip(int fn,Ethernet & ether,int timeout_msec);
void enviar(int fn,BYTE *mensaje, int largo, Ethernet &e,Protocolo &p);
int recibe(int fn,int puerto,BYTE * mensaje,BYTE * macNodo,int timeout_msec,Ethernet &e,Protocolo &p);
//Gestion de nodos
int gestionarNodo(int puerto,Matrices & info,BYTE * macOrigen,int TTL,char * nombre);
void getMacUsr(BYTE* mac,char *Nusr);
bool checkMac(BYTE *mac,BYTE *macusr);

//Tratamiento de matrices
int existeMac( Matrices & info,BYTE *mac);
int buscarEspacioNodo(Matrices & info);
void agregarNombre(int nodo,char *nombre,Matrices & info);
void agregarMac(int nodo,BYTE *macOrigen,Matrices & info);
void actualizarTTL(int nodo,int puerto,int TTL,Matrices & info);
char * NombreDeMac(Matrices & info,BYTE * MAC);
void limpiarTTL(int nodo,Matrices & info);
void limpiarTTLs(Matrices & info);
// Ruteo
int mejorPuertoDestino(int nodo,int cantPuertos,Matrices & info);
bool EstadoNodo(int nodo,int puertos ,Matrices & info);
#endif
