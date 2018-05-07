#include "embellecer.h"

Embellecer::Embellecer(string contentFile, int tabs) {
    tabSize = tabs;
    uglyCode = contentFile;
    ignore = true;
    for(int index = 0; index < 64; index++){
        used[index] = 0;
    }
}

Embellecer::~Embellecer(){

}

void Embellecer::updateCountReservedW(string reserved){
    for(int index = 0; index < 64; index++){
		if(reserved == RESERVED_WORDS[index]){
			used[index]++;
			}
		}
}

void Embellecer::createUsedWords(Buzon &b, long tipo){
    for(int index = 0; index < 64; index++){
        char* converted = new char[(RESERVED_WORDS[index].length())+1];
        strcpy(converted, RESERVED_WORDS[index].c_str());
        b.Enviar(converted, used[index], tipo);
        delete[] converted;
    }
}

void Embellecer::indent(int tabs){
	for(int i = 0; i < tabs; i++){
		prettyCode += ' ';
	}
}

string Embellecer::processContent(){
    prettyCode = "";
	int cuentaSimbolos;
    bool changeLine = true;
	char actual;
	char past;
	string word = "";
	actual = uglyCode[0];
	word += actual;
	int tabs = 0;
	for(int index = 1; index < uglyCode.length(); index++){
		past = prettyCode[prettyCode.length()-1];
        actual = uglyCode[index];
		if(past == '\n' && actual != '\n'){
			indent(tabs);
		}
		if(actual == '#'){
			updateCountReservedW(word);
				prettyCode += word;
				word = "";
			prettyCode += '\n';
		}
		if(actual == '('){
				updateCountReservedW(word);
				prettyCode += word;
				word = "";
				prettyCode += actual;
				changeLine = false;
		} else if(actual == ')'){
				updateCountReservedW(word);
				prettyCode += word;
				word = "";
				prettyCode += actual;
				changeLine = true;
		} else if(actual == ';'){
				updateCountReservedW(word);
				prettyCode += word;
				word = "";
				prettyCode += actual;
				if(changeLine == true && (uglyCode[index-1] != '\'' && uglyCode[index+1] != '\'')){
					prettyCode += '\n';
					indent(tabs);
				} else if(uglyCode[index-1] == '\'' && uglyCode[index+1] != '\''){
					prettyCode += '\n';
					indent(tabs);
				}
		} else if(actual == '{'){
				updateCountReservedW(word);
				prettyCode += word;
				word = "";
				prettyCode += actual;
				if(uglyCode[index-1] != '\'' && uglyCode[index+1] != '\''){
					prettyCode += '\n';
					tabs += tabSize;
					indent(tabs);
				}
		} else if(actual == '}'){
				updateCountReservedW(word);
				prettyCode += word;
				word = "";
				prettyCode += actual;
				if(uglyCode[index+1] == ';'){
					//No se Hace nada.
				} else if(uglyCode[index-1] != '\'' && uglyCode[index+1] != '\''){
					prettyCode += '\n';
					tabs -= tabSize;
					indent(tabs);
				}
		} else if(actual == '/'){
				prettyCode += word;
                updateCountReservedW(word);
                word = "";
                if(past == '/'){
                    prettyCode += actual;
                    int i = index+1;
                    while(uglyCode[i] != '\n'){
                        prettyCode += uglyCode[i];
                        i++;
                    }
                    prettyCode += '\n';
                    index = i;
                } else {
                    prettyCode += actual;
                }
		} else if(actual == '+' || actual == '-' || actual == '=' || actual == '%' || actual == '*'){
				updateCountReservedW(word);
				prettyCode += word;
				word = "";
				prettyCode += actual;
		} else if(actual == '\n'|| actual == '\t' || actual == '\r'){
			//nada se hace.
		} else if(actual == ' '){
				updateCountReservedW(word);
				prettyCode += word;
				word = "";
				prettyCode += actual;
				if(past != ' '){
					prettyCode += actual;
				}
		} else if(actual == ':'){
				updateCountReservedW(word);
				prettyCode += word;
				word = "";
				prettyCode += actual;
				if(!changeLine && uglyCode[index-1] != '\'' && uglyCode[index+1] != '\''){
					prettyCode += '\n';
					indent(tabs);
				}
		} else if(actual == '"'){
				updateCountReservedW(word);
				prettyCode += word;
				int i = index+1;
				word = "";
				prettyCode += actual;
				while(uglyCode[i] != '"'){
					prettyCode += uglyCode[i];
					i++;
				}
				prettyCode += '"';
				index = i;
				if(uglyCode[index+1] == '\n'){
					prettyCode += '\n';
				}
		} else if(actual == '<'){
				updateCountReservedW(word);
				prettyCode += word;
				word = "";
				prettyCode += actual;
		} else if(actual == '>'){
				updateCountReservedW(word);
				prettyCode += word;
				word = "";
				prettyCode += actual;
				if(changeLine && uglyCode[index-1] != '\'' && uglyCode[index+1] != '\'' && uglyCode[index+1] != ':'){
					prettyCode += '\n';
					indent(tabs);
				}
		} else {
			word += actual;
		}
	}
    return prettyCode;
}
