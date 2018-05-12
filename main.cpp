#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "embellecer.h"
#include "Semaforo.h"
using namespace std;

/*
	EFE: Lee el archivo con el codigo a justificar y guarda su contenido en una nueva hilera.
	REQ: file existente.
	MOD: N/A.
*/
string fileContentToString(char* file){
    fstream fileContent (file);
    string contentRead = "";
    string line = "";
    while (getline(fileContent, line)) {
        contentRead = contentRead + line + "\n";
    }
    return contentRead;
}


struct AreaCompartida {		// Estructura usada para la memoria comparida
  struct seccion{//Contenido de la estructura.
     int Revisadas;			// Contiene cuantas veces se ha revisado la palabra asignada al indice de esta palabra.
     int Veces;		// Contiene cuantas veces se ha usado la palabra reservada en todos los documentos.
  } sec[64];
};

typedef struct AreaCompartida AC;

int main(int argc, char** argv) {
  if(argc < 2){
      printf("ERROR: Debe ingresar algun archivo a indentar.\n");
      return -1;
  }
    Embellecer *e;//Embellecedor de codigo.
    Buzon b;//Buzon para la comunicacion entre procesos.
    AC *area;//Puntero para la memoria compartida.
    Semaforo semH(0,0x1B40340);//Semaforo usado para detener el proceso hijo.
    Semaforo semP(0,0x2B40340);//Semaforo usado para detener el proceso padre.
    int tabSize = 4;//Tamano del espacio de tabulacion.
    int filesStart = 1;//Variable que guarda donde inician los archivos.
    char currentChar = 'a';//Guarda el primer caracter de la palabra actual que se esta enviando a imprimir.
    char lastChar = 'a';//Guarda el primer caracter de la primer palabra que se envio anteriormente a imprimir.
    string fileContent = ""; //Variable que guarda la hilera a justificar en caso de que fuera ingresada una hilera.
    int thisId = shmget(0x00B40340, sizeof(AC), 0700 | IPC_CREAT);//Crea id de memoria compartida
    area = (AC*) shmat(thisId, NULL, 0);//Reserva el espacio de memoria compartida.
    string RWA[64] = {      "abstract",    "auto",     "bool",         "break",        "case",             "catch",    "char",
                            "class",       "const",    "const_cast",   "continue",     "decltype",         "default",  "delete",
                            "do",          "double",   "dynamic_cast", "else",         "enum",             "explicit", "extern",
                            "false",       "float",    "for",          "friend",       "goto",             "if",       "inline",
                            "int",         "long",     "mutable",      "namespace",    "new",              "nullptr",  "operator",
                            "private",     "protected","public",       "register",     "reinterpret_cast", "return",   "short",
                            "signed",      "sizeof",   "static",       "static_assert","static_cast",      "struct",   "switch",
                            "template",    "this",     "throw",        "true",         "try",              "typedef",  "typeid",
                            "typename",    "union",    "unsigned",     "using",        "virtual",          "void",     "volatile",
                            "while"};
    /*Frecuencia de la primer letra de las palabras reservadas.
    a = 2, b = 2, c = 7, d = 6, e = 4, f = 4, g = 1, i = 3, l = 1, m = 1, n = 3, o = 1, p = 3,
    r = 3, s = 8, t = 8,u = 3,v = 3, w = 1.
    */
    for(int i = 0; i < 64; i++){//Blanquea la memoria compartida.
        for(int j = 0; j < 2; j++){
            area->sec[i].Revisadas = 0;
            area->sec[i].Veces = 0;
        }
    }
    if(argv[1][0] == '-'){//Si hay modificacion al tamano de tabulacion.
        if(argc == 2){printf("ERROR: Debe ingresar un archivo a indentar.\n"); return -1;}
        string newTabSize = argv[1];
        tabSize = stoi(newTabSize.substr(2));
        filesStart = 2;
      }
    int k = 0;
    if(fork()){//Father
        for(int i = filesStart; i < argc; i++){//Ocupo otro semaforo o se cae porque el padre termina de ejecutarse antes que el
            if(!fork()){//Counting Son
                printf("\nHijo Indentador!\n");
                string outFileName = argv[i];//Variable para guardar el nombre del archivo de salida.
                outFileName += ".sgr"; //Extension agregada al archivo indentado.
                fileContent = fileContentToString(argv[i]);
                e = new Embellecer(fileContent,tabSize, RWA);
                ofstream outFile (outFileName);
                outFile << e->processContent();
                e->createUsedWords(b, (long)i);
                delete e;
                printf("\nArchivo del hijo listo!\n");\
                _exit(0);
            } else {//Father
                printf("\nSoy el padre!\n");
                char rWord[64];
                int wRepetitions;
                for(int index = 0; index < 64; index++){//Se reciben las 64 palabras reservadas del buzon.
                    b.Recibir(rWord, wRepetitions,512, (long)i);
                    for(int indexj = 0; indexj < 64; indexj++){//Se busca la palabra sacada del buzon en la memoria compartida y se aumentan sus valores.
                        if(rWord == RWA[indexj]){
                            area->sec[indexj].Revisadas++;
                            area->sec[indexj].Veces += wRepetitions;
                            indexj = 64;
                            currentChar = RWA[index][0];
                            if(currentChar <= lastChar && area->sec[index].Revisadas == argc-filesStart){//Se pausa la lectura del buzon del padre para imprmir.
                                lastChar = currentChar;
                                semH.Signal();//Se permite que el hijo imprima.
                                semP.Wait();//Se Duerme el padre mientras el hijo imprime.
                            }
                        }
                    }
                    currentChar='a';//Se reinicia el valor del primer caracter que se quiera imprimir.
                }
                printf("\nFin del padre!\n");
                if(i==argc-1){semH.Signal();}//Despierta al hijo que imprime para que el programa pueda terminar.
            }
        }
    } else {//Printing Son.
        semH.Wait();//Se espera hasta que el padre de permiso de imprimir.
        printf("Hijo Impresor!\n");
        for(int index = 0; index < 64; index++){
            currentChar = RWA[index][0];
            if(currentChar <= lastChar){//Si el caracter que se va a imprimir es distinto al anterior se pausa la impresion.
              lastChar = currentChar;
              semP.Signal();
              semH.Wait();
            }
            if(area->sec[index].Veces != 0){//Imprime en pantalla las palabras reservadas cuya frequencia no es cero.
                printf("< %s, %i >\n",RWA[index].c_str(),area->sec[index].Veces);
            }
        }
        //semP.Signal();//Se despierta al padre.
        cout<<"Fin Impresion!\n";
        _exit(0);
    }
    shmdt(area);//Se hace detach de la memoria compartida.
    shmctl(thisId, IPC_RMID, NULL);//Se elimina el espacio de memoria compartida.
}
