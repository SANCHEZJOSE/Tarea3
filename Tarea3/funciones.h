#ifndef FUNCIONES_H
#define FUNCIONES_H
#define BYTE unsigned char
#include "funcionesStruct.h"
///Prototipos
//									mac 1       mac 1
void enviar(BYTE *frame, int largo,int destino, int origen);
int recibe(BYTE * mensaje,int timeout_msec,int origen);
void empaquetarProtocolo(Protocolo &);
void getMac(BYTE* mac, int n);
bool checkMac(BYTE *mac,int nodo);
//Tratamiento de matrices
int existeMac( int m[8][6] ,int m2[6],int N );
void agregarMac(int m[8][6], int m2[6], int pos);
void agregarNombre(char nombres[nodos][10],char newName[10],int pos);
void ActualizarTtl(int ttl[nodos][3],int newTtl,int nodo,int puerto);
#endif