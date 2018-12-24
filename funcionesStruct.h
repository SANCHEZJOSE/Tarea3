#ifndef FUNCIONES_STRUCT_H
#define FUNCIONES_STRUCT_H
#define TAM 126
#define NODOS 15

struct Protocolo{
	int cmd;//3 bits --> 8 comandos
	int Long;//7 bits --> 127 bits maximo
	int ttl;//4 bits -->max 15 nodos
	unsigned char data[TAM];//126 --> 126 caracteres maximo
	int fcs;//10 bits -->1023 bits
	unsigned char frame[TAM+3];//129
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
	int ttl[NODOS][4];//ttl de cada nodo
	unsigned char mac[NODOS][6];//mac de cada nodos
	char nombres[NODOS][12];//nombre de los nodos
	int contBroadcast[NODOS];//Permite almacenar la cantidad de veces 
	//que llega un broadcast en cierta cantidad de procesos automaticos.
	int flag;//permite obtener posicion del ultimo nodo agregado
	Matrices(){
		for(int i=0;i<NODOS;i++){
			for(int j=0;j<4;j++){
				ttl[i][j]=-1;
			}
			for(int x=0;x<6;x++){
				mac[i][x]=0;
			}
			for(int z=0;z<10;z++){
				nombres[i][z]='\0';
			}
			contBroadcast[i]=0;
		flag=0;
		}
	}
};
#endif
