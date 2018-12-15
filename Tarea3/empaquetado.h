#ifndef FUNCIONES_H
#define FUNCIONES_H
#define BYTE unsigned char
#include "funcionesStruct.h"

///Prototipos
//									mac 1       mac 1
void enviar(BYTE *frame, int largo,int destino, int origen);
int recibe(BYTE * mensaje,int timeout_msec,int origen);
void empaquetarProtocolo(Protocolo &);
bool desempaquetarProtocolo(Protocolo &);
int empaquetarEthernet(Protocolo &,Ethernet &);
bool desempaquetarEthernet(Protocolo &,Ethernet &);
void writeSlip(int,Ethernet& );
void readSlip(int,Ethernet& ,int);
void getMac(BYTE* mac, int n);
bool checkMac(BYTE *mac,int nodo);
int fcs(BYTE *x, int n);
int fcs (BYTE x);

#endif