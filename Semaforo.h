#ifndef SEMAFORO_H
#define SEMAFORO_H
#define KEY 0xB40340
#include <cstdio>
#include <unistd.h>
using namespace std;
class Semaforo{
public:
    Semaforo( int ValorInicial = 0);
    ~Semaforo();
    int Signal();	// 
    int Wait();	
    
private:
    int id;	
    union semun {
        int              val;    /* Value for SETVAL */
        struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
        unsigned short  *array;  /* Array for GETALL, SETALL */
        struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific)*/
    };
};
#endif /* SEMAFORO_H */

