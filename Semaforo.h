#ifndef SEMAFORO_H
#define SEMAFORO_H
#include <cstdio>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
using namespace std;
class Semaforo{
public:
    Semaforo( int ValorInicial, key_t KEY);
    ~Semaforo();
    int Signal();	//
    int Wait();

private:
    int id;
    key_t llaves;
    union semun {
        int              val;    /* Value for SETVAL */
        struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
        unsigned short  *array;  /* Array for GETALL, SETALL */
        struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific)*/
    };
};
#endif /* SEMAFORO_H */
