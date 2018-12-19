#ifndef FUNCIONES_STRUCT_H
#define FUNCIONES_STRUCT_H
#define tam 126
#define nodos 9

struct Protocolo{
	int cmd;//3 bits --> 7 comandos
	int Long;//7 bits --> 127 bits maximo
	int ttl;//4 bits -->max 15 nodos
	unsigned char data[tam];//126 --> 126 caracteres maximo
	int fcs;//10 bits -->1023 bits
	unsigned char frame[tam+3];//130
};
struct Ethernet{
	unsigned char MACD[6];
	unsigned char MACO[6];
	int Long;//2BYTES
	//unsigned char DATA[N+3];
	int fcs;//4BYTES
	unsigned char frameEth[148];
};
struct Matrices{
	int ttl[nodos][4];//ttl de cada nodo
	unsigned char mac[nodos][6];//mac de cada nodos
	char nombres[nodos][10];//nombre de los nodos
	Matrices(){
		for(int i=0;i<nodos;i++){
			for(int j=0;j<4;j++){
				ttl[i][j]=-1;
			}
			for(int x=0;x<6;x++){
				mac[i][x]=0;
			}
			for(int z=0;z<10;z++){
				nombres[i][z]='\0';
			}
		}
	}
};
#endif
