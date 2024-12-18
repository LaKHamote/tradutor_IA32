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

Table<int, string> imaginary_instructions({
    {1, "ADD"},
    {2, "SUB"},
    {3, "MUL"},
    {4, "DIV"},
    {5, "JMP"},
    {6, "JMPN"},
    {7, "JMPP"},
    {8, "JMPZ"},
    {9, "COPY"},
    {10, "LOAD"},
    {11, "STORE"},
    {12, "INPUT"},
    {13, "OUTPUT"},
    {14, "STOP"}
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
    {"INPUT", callinputFunction},
    {"OUTPUT", calloutputFunction},
    {"STOP", stopFunction}
});

Table<int, string> labels;
Table<string, int> used_labels;

int main(int argc, char* argv[]) {
    function<void(string, ostream&)> translate_IA32;
    int CURRENT_STATE = OPCODE_STATE; // começa procurando opcodes

    ifstream inputFile(argv[1]);
    string inputFilePath = argv[1];
    if (!inputFile.is_open()) {
        throw invalid_argument("\n\n\tArquivo nao encontrado\n");
    }
    string pathToFile = inputFilePath.substr(0, inputFilePath.length() - 3); // retirar obj
    ofstream outputFileTemp(pathToFile+"tmp");
    ifstream inputFileTemp(pathToFile+"tmp");
    ofstream outputFile(pathToFile+"s");
    string line;
    if (inputFile.is_open()) {
        getline(inputFile, line); // o codigo possui apenas uma linha com instrucoes
        // cout << line << "\n";
        string num_read;
        int instr;
        int first_label_address=10000;
        string opcode;
        string imaginary_instr;
        bool copy_inst=false;
        bool branch_inst=false;
        int copy_param=-1;
        int mem_address = 0;
        int label_counter=0;
        istringstream iss(line);// iss>>instr gets the next instr
        Table<int, int> consts;
        Table<int, int> spaces;
        while(iss>>num_read){
            // cout << mem_address <<"\n";
            // cout << instr <<"\n";
            instr = stoi(num_read);
            if(mem_address==first_label_address) CURRENT_STATE=DATA_STATE;
            
            if(CURRENT_STATE==OPCODE_STATE){
                imaginary_instr = *imaginary_instructions.get(instr);
                opcode=imaginary_instr;
                // cout << opcode <<"\n";
                translate_IA32 = *ia32_instructions.get(imaginary_instr);
                labels.add(mem_address, "LABEL"+to_string(label_counter++));
                outputFileTemp << *labels.get(mem_address) << ":\n";

                if((5<=instr)&&(instr<=8)) branch_inst=true;

                if(imaginary_instr=="COPY")  copy_inst=true; // copy tem dois operands

                if(imaginary_instr=="STOP"){
                    stopFunction("",outputFileTemp); // stop nao tem argumento
                }else{
                    CURRENT_STATE=OPERAND_STATE;
                }
            }else if(CURRENT_STATE==OPERAND_STATE){
                if(NOT (branch_inst)&&(instr<first_label_address)) first_label_address=instr;
                branch_inst=false; // resetar a flag todo loop

                if (labels.get(instr)==nullptr) labels.add(instr, "LABEL"+to_string(label_counter++));
                if (used_labels.get(*labels.get(instr)+":")==nullptr) used_labels.add(*labels.get(instr)+":", instr);
                
                if(NOT copy_inst){                    
                    string* param0_ptr = labels.get(copy_param);
                    string* param1_ptr = labels.get(instr);
                    if(param0_ptr!=nullptr){//nesse caso precisamos passar o outro parametro do copy
                        translate_IA32(*param0_ptr +","+ *param1_ptr, outputFileTemp);
                    }else translate_IA32(*param1_ptr, outputFileTemp);
                    copy_param=-1;
                    CURRENT_STATE=OPCODE_STATE;
                }else{
                    copy_param=instr;
                    copy_inst=false;
                }
            }else if(CURRENT_STATE==DATA_STATE){
                if(instr!=0){//instr initialized
                    consts.add(mem_address, instr);
                    // cout << "\t" <<*labels.get(mem_address) << " dd "<< instr << "\n";
                }else{//instr not initialized
                    spaces.add(mem_address, instr);
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
        outputFile<<"\t\tstr_lido db \"Foram lidos \", 0 \n";
        outputFile<<"\t\tlen_lido equ $-str_lido \n";

        outputFile<<"\t\tstr_bytes db \" bytes\", 0xA \n";
        outputFile<<"\t\tlen_bytes equ $-str_bytes\n";

        outputFile<<"\t\tstr_escrito db \"Foram escritos \", 0 \n";
        outputFile<<"\t\tlen_escrito equ $-str_escrito \n";

        outputFile<<"\t\toverflow_msg db \"Ocorreu overflow numa multiplicação \", 0 \n";
        outputFile<<"\t\toverflow_msg_len equ $-overflow_msg \n";

        for (const auto &elem : *consts.getData()) {
            outputFile << "\t\t" <<*labels.get(elem.first) << " dd "<< elem.second << "\n";
        }

        outputFileTemp.close();
        if (inputFileTemp.is_open()) {
            init(outputFile);
            while(getline(inputFileTemp, line)){
                if( (line.back()!=':') ||
                    (used_labels.get(line)!=nullptr)
                ){
                    outputFile<<line<<endl;
                }

            }
        
        }else cerr << "Erro ao abrir o arquivo de saída" << endl;

        
        // labels.show();
        // cout<< "---------------" <<"\n";
        // used_labels.show();
        inputFileTemp.close();
        inputFile.close();
        writeinputFunction(outputFile); //adiciona--funcao--de--input
        
        writeoutputFunction(outputFile); //adiciona--funcao--de--output

        outputFile<<"\n\noverflow_detected:\n";
        outputFile << "\t\tmov eax, 4\n";
        outputFile << "\t\tmov ebx, 1\n";
        outputFile << "\t\tmov ecx, overflow_msg\n";
        outputFile << "\t\tmov edx, overflow_msg_len\n";
        outputFile << "\t\tint 0x80\n";
        stopFunction("", outputFile);
        
        outputFile.close();

    }else {
        cerr << "Não foi possível abrir o arquivo." << "\n";
    }
    return 0;
}