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
#define DATA_STATE 2

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
void myFunction(string param, ostream& output ) {
    cout << "Function " << param << "\n";
}

Table<string, function<void(string, ostream&)>> ia32_instructions({
    {"ADD", addFunction},
    {"SUB", subFunction},
    {"MUL", multFunction},
    {"DIV", divFunction},
    {"JMP", jumpFunction},
    {"JMPN", jumpNegFunction},
    {"JMPP", jumpPosFunction},
    {"JMPZ", jumpZeroFunction},
    {"COPY", copyFunction},
    {"LOAD", loadFunction},
    {"STORE", storeFunction},
    {"INPUT", myFunction},
    {"OUTPUT", myFunction},
    {"STOP", stopFunction}
});

Table<int, string> labels;
Table<string, int> used_labels;

int main() {
    function<void(string, ostream&)> translate_IA32;
    int CURRENT_STATE = OPCODE_STATE; // começa procurando opcodes
    ifstream inputFile("../examples/ex1.o");
    ofstream outputFileTemp("../examples/saida1.tmp");
    ifstream inputFileTemp("../examples/saida1.tmp");
    ofstream outputFile("../examples/saida1.asm");
    if (!outputFileTemp.is_open()) {
        cerr << "Erro ao criar o arquivo temporario" << endl;
    }
    string line;
    if (inputFile.is_open()) {
        getline(inputFile, line); // o codigo possui apenas uma linha com instrucoes
        // cout << line << "\n";
        string instr;
        string opcode;
        string imaginary_instr;
        bool copy_inst;
        string copy_param="-1";
        int mem_address = 0;
        int label_counter=0;
        int data;
        istringstream iss(line);// iss>>instr gets the next instr
        Table<int, int> consts;
        Table<int, int> spaces;
        init(outputFileTemp);
        while(iss>>instr){
            // cout << mem_address <<"\n";
            // cout << instr <<"\n";
            
            if(CURRENT_STATE==OPCODE_STATE){
                imaginary_instr = *imaginary_instructions.get(instr);
                opcode=imaginary_instr;
                // cout << opcode <<"\n";
                translate_IA32 = *ia32_instructions.get(imaginary_instr);
                labels.add(mem_address, "LABEL"+to_string(label_counter++));
                outputFileTemp << *labels.get(mem_address) << ":\n";

                if(imaginary_instr=="COPY")  copy_inst=true; // copy tem dois operands

                if(imaginary_instr=="STOP"){
                    stopFunction("",outputFileTemp);
                    CURRENT_STATE=DATA_STATE;

                }else{
                    CURRENT_STATE=OPERAND_STATE;
                }
            }else if(CURRENT_STATE==OPERAND_STATE){
                if(NOT copy_inst){                    
                    if (labels.get(stoi(instr))==nullptr) labels.add(stoi(instr), "LABEL"+to_string(label_counter++));
                    if (used_labels.get(*labels.get(stoi(instr))+":")==nullptr) used_labels.add(*labels.get(stoi(instr))+":", stoi(instr));
                    string* param0_ptr = labels.get(stoi(copy_param));
                    string* param1_ptr = labels.get(stoi(instr));
                    if(param0_ptr!=nullptr){//nesse caso precisamos passar o outro parametro do copy
                        translate_IA32(*param0_ptr +","+ *param1_ptr, outputFileTemp);
                    }else translate_IA32(*param1_ptr, outputFileTemp);
                    copy_param="-1";
                    CURRENT_STATE=OPCODE_STATE;
                }else{
                    copy_param=instr;
                    copy_inst=false;
                }
            }else if(CURRENT_STATE==DATA_STATE){
                data = stoi(instr);
                if(data!=0){//data initialized
                    consts.add(mem_address, data);
                    // cout << "\t" <<*labels.get(mem_address) << " dd "<< data << "\n";
                }else{//data not initialized
                    spaces.add(mem_address, data);
                    // cout<<"section .bss\n";
                    // cout << "\t" << *labels.get(mem_address++) <<" resd " << "1\n";
                }
            }
        // cout<<"-----------"<<"\n";
        mem_address++;
        }

        if (!outputFile.is_open()) {
            cerr << "Erro ao criar o arquivo temporario" << endl;
        }

        outputFile<<"section .bss\n";
        for (const auto &elem : *spaces.getData()) {
            outputFile << "\t\t" << *labels.get(elem.first) <<" resd " << "1\n";
        }
        outputFile<<"\nsection .data\n";
        for (const auto &elem : *consts.getData()) {
            outputFile << "\t\t" <<*labels.get(elem.first) << " dd "<< elem.second << "\n";
        }

        outputFileTemp.close();
        if (inputFileTemp.is_open()) {
            outputFile<<"\n";
            while(getline(inputFileTemp, line)){
                if( (line.back()!=':') ||
                    (used_labels.get(line)!=nullptr)
                ){
                    outputFile<<line<<endl;
                }

            }
        
        }else cerr << "Erro ao abrir o arquivo de saída" << endl;

        
        labels.show();
        cout<< "---------------" <<"\n";
        used_labels.show();
        inputFileTemp.close();
        inputFile.close();
        outputFile.close();

    }else {
        cerr << "Não foi possível abrir o arquivo." << "\n";
    }
    return 0;
}