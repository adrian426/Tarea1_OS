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
/*
	EFE: Solicita datos al usuario desde consola hasta que ingrese la palabra *terminate en una nueva linea.*
	REQ: N/A.
	MOD: N/A.
*/
string getFromConsole(){
	string readCode = "";
	string line = "";
	bool terminate = false;
	cout<<"Ingrese el codigo que desea justificar.\n Escriba *terminate* en una nueva linea para detener la solicitud de texto.\n";
		while(getline(std::cin, line) && line != "*terminate*"){//Solicitud de datos.
			readCode += line;
			readCode += "\n";
		}

	return readCode;
}

struct shMData{
    int used[64][2];
    /*
    const string RESERVED_WORDS[64] = {"abstract",    "auto",     "bool",         "break",        "case",             "catch",    "char",
                                      "class",       "const",    "const_cast",   "continue",     "decltype",         "default",  "delete",
                                       "do",          "double",   "dynamic_cast", "else",         "enum",             "explicit", "extern",
                                       "false",       "float",    "for",          "friend",       "goto",             "if",       "inline",
                                       "int",         "long",     "mutable",      "namespace",    "new",              "nullptr",  "operator",
                                       "private",     "protected","public",       "register",     "reinterpret_cast", "return",   "short",
                                       "signed",      "sizeof",   "static",       "static_assert","static_cast",      "struct",   "switch",
                                       "template",    "this",     "throw",        "true",         "try",              "typedef",  "typeid",
                                       "typename",    "union",    "unsigned",     "using",        "virtual",          "void",     "volatile",
                                       "while"};
                                       a = 2,
                                       b = 2,
                                       c = 7,
                                       d = 6,
                                       e = 4,
                                       f = 4,
                                       g = 1,
                                       i = 3,
                                       l = 1,
                                       m = 1,


 */

};

int main(int argc, char** argv) {
    int tabSize = 4;
    Embellecer *e;
    Buzon b;
    Semaforo s;
    int thisId = shmget(0x00B40340, sizeof(int[64][2]), 0700 | IPC_CREAT);
    int* area = (int*) shmat(thisId, NULL, 0);
    const string RWA[64] = {"abstract",    "auto",     "bool",         "break",        "case",             "catch",    "char",
                                      "class",       "const",    "const_cast",   "continue",     "decltype",         "default",  "delete",
                                       "do",          "double",   "dynamic_cast", "else",         "enum",             "explicit", "extern",
                                       "false",       "float",    "for",          "friend",       "goto",             "if",       "inline",
                                       "int",         "long",     "mutable",      "namespace",    "new",              "nullptr",  "operator",
                                       "private",     "protected","public",       "register",     "reinterpret_cast", "return",   "short",
                                       "signed",      "sizeof",   "static",       "static_assert","static_cast",      "struct",   "switch",
                                       "template",    "this",     "throw",        "true",         "try",              "typedef",  "typeid",
                                       "typename",    "union",    "unsigned",     "using",        "virtual",          "void",     "volatile",
                                       "while"};
    for(int i = 0; i < 64; i++){//Blanquea la memoria compartida.
      for(int j = 0; j < 2; j++){
        area[i*2+j] = 0;
      }
    }
    string tabSizeStr = "";//variable que guarda la cantidad de espacios de tabulación en caso que sean ingresados en la ejecución.
    string received = ""; //Variable que guarda la hilera con el código a justificar en caso que sea por la entrada estandar..
    string fileContent = ""; //Variable que guarda la hilera a justificar en caso de que fuera ingresada una hilera.
    string fInstruction = ""; //Variable que se usa para obtener los datos que da el usuario en comandos.
  for(int i = 1; i < argc; i++){
      if(!fork()){
          if(i == 0){//Printing Son
           printf("Impresor");
            //s.Wait();
            for(int index = 0; index < 64; index++){
              if(area[index*2] != 0){
                cout<<"<"<<RWA[i]<<", "<< area[index*2]<<">\n";
                //printf("<%s, %i>\n",RWA[i],area[index*2+0]);
              }
            }
            _exit(0);
          } else {//indentation son.
           printf("\nSoy el hijo #%i!\n",i);
           string outFileName = argv[i];
           outFileName += ".sgr"; //Variable para nombrar guardar el nombre del archivo de salida.
           fileContent = fileContentToString(argv[1]);
           e = new Embellecer(fileContent,tabSize);
           ofstream outFile (outFileName);
           outFile << e->processContent();
           e->createUsedWords(b, (long)i);
           delete e;
           _exit(0);
          }
	     } else {//El tata.
      		char rWord[512];
      		int wRepetitions;
      		for(int index = 0; index < 64; index++){
      			b.Recibir(rWord, wRepetitions,512, (long)i);/*
      			if(wRepetitions != 0){
      				printf("<%s, %i>\n",rWord,wRepetitions);
            }*/
            for(int index = 0; index < 64; index++){
                if(rWord == RWA[index]){
                  area[index*2+0] += wRepetitions;
                  area[index*2+1]++;
                  index = 64;
                }
              }
      			}
            //s.Signal();
         }
  		}
      shmdt(area);
      shmctl(thisId, IPC_RMID, NULL);
  }

/* main de la tarea 0
int main(int argc, char** argv) {
    int tabSize = 4;
    Embellecer *e;
    string tabSizeStr = "";//variable que guarda la cantidad de espacios de tabulación en caso que sean ingresados en la ejecución.
    string received = ""; //Variable que guarda la hilera con el código a justificar en caso que sea por la entrada estandar..
    string fileContent = ""; //Variable que guarda la hilera a justificar en caso de que fuera ingresada una hilera.
    string outFileName = "rst.txt"; //Variable para nombrar guardar el nombre del archivo de salida.
    string fInstruction = ""; //Variable que se usa para obtener los datos que da el usuario en comandos.
    ofstream x ("cuentaDePalabrasReservadas.txt"); //Creación del archivo con la cuenta del uso de palabras reservadas.

    switch(argc){
        case 1: //Sin argumentos.
        {
			received = getFromConsole();
            e = new Embellecer(received, tabSize);
            ofstream outFile (outFileName);
            outFile << e->processContent();
			cout<<e->processContent();
            x << e->createUsedWords();
        }
            break;
        case 2:
        {
            if(argv[1][0]=='-'){
                if(argv[1][1] == 'e'){//-eX
                    fInstruction = argv[1];
					tabSizeStr = fInstruction.substr(2);
					tabSize = stoi(tabSizeStr);
					if(tabSize < 0){
						cout<<"No se aceptan numeros negativos para tabular.\n";
						return -1;
					}
                } else {//-oX
                    fInstruction = argv[1];
                    outFileName = fInstruction.substr(2);
                }

                received = getFromConsole();
                e = new Embellecer(received, tabSize);
                ofstream outFile (outFileName);
                outFile << e->processContent();
                x << e->createUsedWords();
            } else {//fileToJustify
                fileContent = fileContentToString(argv[1]);
                e = new Embellecer(fileContent,tabSize);
                ofstream outFile (outFileName);
                outFile << e->processContent();
                x << e->createUsedWords();
            }
        }
            break;
        case 3:
        {
            if(argv[1][0] == '-'){
                if(argv[2][0] == '-'){//-eX -o
					fInstruction = argv[1];
					tabSizeStr = fInstruction.substr(2);
					tabSize = stoi(tabSizeStr);
					if(tabSize < 0){
						cout<<"No se aceptan numeros negativos para tabular.\n";
						return -1;
					}
                    fInstruction = argv[2];
                    outFileName = fInstruction.substr(2);

                    received = getFromConsole();

                    e = new Embellecer(received, tabSize);
                    ofstream outFile (outFileName);
                    outFile << e->processContent();
                    x << e->createUsedWords();
                } else {//-eX fileToJustify.
                    fInstruction = argv[1];
					tabSizeStr = fInstruction.substr(2);
					tabSize = stoi(tabSizeStr);
					if(tabSize < 0){
						cout<<"No se aceptan numeros negativos para tabular.\n";
						return -1;
					}
                    fileContent = fileContentToString(argv[2]);
                    e = new Embellecer(fileContent,tabSize);
                    ofstream outFile (outFileName);
                    outFile << e->processContent();
                    x << e->createUsedWords();
                }
            } else {//fileToJustify -o
                    fInstruction = argv[2];
                    fileContent = fileContentToString(argv[1]);
                    e = new Embellecer(fileContent,tabSize);
                    outFileName = fInstruction.substr(2);
                    ofstream outFile (outFileName);
                    outFile << e->processContent();
                    x << e->createUsedWords();
            }
            break;
        }
        case 4:
        {//-eX fileToJustify -oX
			fInstruction = argv[1];
			tabSizeStr = fInstruction.substr(2);
			tabSize = stoi(tabSizeStr);
					if(tabSize < 0){
						cout<<"No se aceptan numeros negativos para tabular.\n";
						return -1;
					}
			cout << tabSizeStr <<endl;
			cout << tabSize <<endl;
            fInstruction = argv[3];
            fileContent = fileContentToString(argv[2]);
            e = new Embellecer(fileContent,tabSize);
            outFileName = fInstruction.substr(2);
            ofstream outFile (outFileName);
            outFile << e->processContent();
            x << e->createUsedWords();
        }
            break;
    }
    return 0;
}
*/
