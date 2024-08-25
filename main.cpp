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
void myFunction(string param ) {
    cout << "Function " << param << "\n";
}

Table<string, function<void(string)>> ia32_instructions({
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

Table<string, string> labels;

int main() {
    function<void(string)> translate_IA32;
    int CURRENT_STATE = OPCODE_STATE; // começa procurando opcodes
    ifstream file("../examples/ex1.asm"); // TODO: fazer IOStream
    string line;
    if (file.is_open()) {
        getline(file, line); // o codigo possui apenas uma linha com instrucoes
        // cout << line << "\n";
        string instr;
        string opcode;
        string imaginary_instr;
        bool copy_inst;
        string copy_param;
        string* label_ptr;
        int mem_address = 0;
        int label_counter=0;

        istringstream iss(line);                                                 // iss>>instr gets the next instr
        while(iss>>instr){
            // cout << mem_address <<"\n";
            // cout << instr <<"\n";
            if(CURRENT_STATE==OPCODE_STATE){
                imaginary_instr = *imaginary_instructions.get(instr);
                opcode=imaginary_instr;
                // cout << opcode <<"\n";
                translate_IA32 = *ia32_instructions.get(imaginary_instr);

                if(imaginary_instr=="COPY")  copy_inst=true; // copy tem dois operands

                if(imaginary_instr=="STOP") break; // resolver macros


                CURRENT_STATE=OPERAND_STATE;

            }else if(CURRENT_STATE==OPERAND_STATE){
                if(NOT copy_inst){

                    label_ptr = labels.get(instr);
                    if(label_ptr==nullptr){
                        labels.add(instr, "label"+to_string(label_counter));
                        label_counter++;
                    }
                    //TODO: escrever linha
                    string* param0_ptr = labels.get(copy_param);
                    string* param1_ptr = labels.get(instr);
                    if(param0_ptr!=nullptr){//nesse caso precisamos passar o outro parametro do copy
                        translate_IA32(*param0_ptr +" "+ *param1_ptr);
                    }else translate_IA32(*param1_ptr);
                    copy_param="";
                    CURRENT_STATE=OPCODE_STATE;
                }else{
                    copy_param=instr;
                    copy_inst=false;
                }
            }
        // cout<<"-----------"<<"\n";
        mem_address++;
        }
        labels.show();
        cout<< "---------------" <<"\n";       
        init(); 
        file.close();
    }else {
        cerr << "Não foi possível abrir o arquivo." << "\n";
    }
    return 0;
}