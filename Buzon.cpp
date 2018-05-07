#include "Buzon.h"

Buzon::Buzon(){
	int x = msgget(KEY, 0600|IPC_CREAT);
	if(-1 == x){
		perror("Creando el Buzon");
		_exit(-1);
		}
	id = x;
}

Buzon::~Buzon(){
	struct msqid_ds strctr;
	int x = msgctl(id, IPC_RMID, &strctr);
	if(-1 == x){
		perror("Destruyendo el Buzon");
		_exit(-1);
		}
}

int Buzon::Enviar(char* mensaje, int cantidad, long tipo){
	msgbuf elMsg;
	elMsg.mtype = tipo;
	elMsg.veces = cantidad;
	strcpy(elMsg.mtext, mensaje);
	elMsg.veces = cantidad;
	if(-1 == msgsnd( id, (void*) &elMsg , 512, 0 )){
		perror("Enviando el mensaje");
		_exit(-1);
	}
}

int Buzon::Recibir(char* mensaje, int& cantidad,int len, long tipo){
	msgbuf elMsg;
	if(-1 == msgrcv( id ,(void*) &elMsg, len, tipo, 0)){
		perror("Recibiendo el mensaje");
		_exit(-1);
	} else {
		strcpy(mensaje, elMsg.mtext);
		cantidad = elMsg.veces;
	}
	return 0;
}
