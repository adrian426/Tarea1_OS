#ifndef EMBELLECER_H
#define EMBELLECER_H
#include <iostream>
#include <string>
#include <fstream>
#include "Buzon.h"
using namespace std;
class Embellecer{
public:
    /*
     * EFE: Inicializa los valores necesarios para empezar a procesar el código.
     * REQ: Hilera de caracteres con el código a justificar, una hilera para el
     * nombre del archivo de salida y un entero para la cantidad de espacios a
     * usar como tabulador.
     * MOD: N/A.
     */
    Embellecer(string content,int tabSize);

    /*
     * EFE: Libera Espacio.
     * REQ: N/A.
     * MOD: Embellecer.
     */
    ~Embellecer();

    /*
     * EFE: Aumenta en 1 el contador de la palabra ingresada en caso de existir en la lista de palabras reservadas.
     * REQ: Hilera válida.
     * MOD: used.
     */
    void updateCountReservedW(string reserved);

    /*
     * EFE: Crea la hilera con la cuenta de palabras reservadas usadas y la retorna.
     * REQ: N/A.
     * MOD: N/A.
     */
    void createUsedWords(Buzon &b, long tipo);

	/*
     * EFE: Agrega los "tabs" a la hilera.
     * REQ: cantidad actual de espacios a agregar.
     * MOD: prettyCode.
     */
    void indent(int tabs);

	/*
     * EFE: Lee la hilera a justificar, la guarda en otra donde su resultado ya está justificado y la retorna.
     * REQ: N/A.
     * MOD: prettyCode.
     */
    string processContent();

private:
	int used[64];
	bool ignore;
	int tabSize;
	string uglyCode;
	string prettyCode;
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
	};
#endif /* EMBELLECER_H */
