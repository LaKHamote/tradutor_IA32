#include "main.hpp"

#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <regex>
#include <functional>

using namespace std;


#define OPCODE_STATE 0
#define OPERAND_STATE 1
#define NOT !

Table<string, string> imaginary_instructions({
        {"1", "ADD"},
        {"2", "SUB"},
        {"3", "MUL"},
        {"4", "DIV"},
        {"5", "JMP"},
        {"6", "JMPN"},
        {"7", "JMPP"},
        {"8", "JMPZ"},
        {"9", "COPY"},
        {"10", "LOAD"},
        {"11", "STORE"},
        {"12", "INPUT"},
        {"13", "OUTPUT"},
        {"14", "STOP"}
});

// A sample function to be used in the map
void myFunction() {
    cout << "Function called!" << endl;
}

Table<string, function<void()>> ia32_instructions({
        {"ADD", myFunction},
        {"SUB", myFunction},
        {"MUL", myFunction},
        {"DIV", myFunction},
        {"JMP", myFunction},
        {"JMPN", myFunction},
        {"JMPP", myFunction},
        {"JMPZ", myFunction},
        {"COPY", myFunction},
        {"LOAD", myFunction},
        {"STORE", myFunction},
        {"INPUT", myFunction},
        {"OUTPUT", myFunction},
        {"STOP", myFunction}
});

int main() {
    function<void()> translate_IA32;
    int CURRENT_STATE = OPCODE_STATE; // começa procurando opcodes
    ifstream file("../examples/ex1.asm"); // TODO: fazer IOStream
    string line;
    if (file.is_open()) {
        getline(file, line); // o codigo possui apenas uma linha com instrucoes
        // cout << line << "\n";
        istringstream iss(line);                                                 // iss>>instr gets the next instr
        string instr;
        string imaginary_instr;
        bool copy_inst;
        int mem_address = 0;
        while(iss>>instr){
            cout << mem_address <<"\n";
            cout << instr <<"\n";
            if(CURRENT_STATE==OPCODE_STATE){
                imaginary_instr = *imaginary_instructions.get(instr);
                cout << imaginary_instr <<"\n";

                if(imaginary_instr=="COPY")  copy_inst=true; // copy tem dois operands

                if(imaginary_instr=="STOP") break; // resolver macros

                translate_IA32 = *ia32_instructions.get(imaginary_instr);
                translate_IA32();

                CURRENT_STATE=OPERAND_STATE;
            }else if(CURRENT_STATE==OPERAND_STATE){


                if(NOT copy_inst){
                    //TODO: escrever linha
                    CURRENT_STATE=OPCODE_STATE;
                }else copy_inst=false;
            }
        cout<<"-----------"<<"\n";
        mem_address++;
        }
        
        file.close();
    }else {
        cerr << "Não foi possível abrir o arquivo." << "\n";
    }
    return 0;
}