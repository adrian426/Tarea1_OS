#ifndef BUZON_H
#define BUZON_H
#define KEY 0xB40340
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
using namespace std;

class Buzon{
public:
    Buzon();
    ~Buzon();
    int Enviar(char* mensaje, int cantidad,long tipo);
    int Recibir(char* mensaje, int& cantidad, int len, long tipo);   // len es el tamaño máximo que soporte la variable mensaje

private:
    int id;	// Identificador del buzon
    struct msgbuf {
               long mtype;       /* message type, must be > 0 */
		           int veces;
               char mtext[512];    /* message data */
           };

};
#endif /* SEMAFORO_H */
