#include "Semaforo.h"  
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

Semaforo::Semaforo(int ValorInicial){
     id = semget(KEY,1,IPC_CREAT|0600);
     if (-1 == id){
          perror("Creando el semaforo.");
          _exit(-1);
     } else {
          semun structure;
          structure.val = 0;
          if ( -1 == semctl(id,0,SETVAL,structure)){
               perror("Dando valor al semaforo.");
               _exit(-1);
          }
}
}

Semaforo::~Semaforo(){
    if(-1 == semctl(id,0,IPC_RMID)){
          perror("Destruyendo el semaforo.");
          _exit(-1);
}
}

int Semaforo::Signal(){
    struct sembuf n;
     n.sem_num = 0;
     n.sem_op = 1;
     n.sem_flg = 0;
     if(-1 == semop(id,&n,1)){
          perror("Recibiendo señal.");
          _exit(-1);
		}
}

int Semaforo::Wait(){
    struct sembuf n;
     n.sem_num = 0;
     n.sem_op = -1;
     n.sem_flg = 0;
     if(-1 == semop(id,&n,1)){
          perror("Esperando señal.");
          _exit(-1);
		}
}
