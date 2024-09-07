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
    {14, "STOP"},
    {15, "S_INPUT"},
    {16, "S_OUTPUT"}
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
    {"STOP", stopFunction},
    {"S_INPUT", callStrInputFunction},
    {"S_OUTPUT", callStrOutputFunction}
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
        int first_data_address=10000;
        string opcode;
        string imaginary_instr;
        bool dbl_oper_inst=false;
        bool copy_inst=false;
        bool s_IO_inst=false;
        bool branch_inst=false;
        int secondary_param=-1;
        int mem_address = 0;
        int label_counter=0;
        istringstream iss(line);// iss>>instr gets the next instr
        Table<int, int> consts;
        Table<int, int> spaces;
        while(iss>>num_read){
            // cout << mem_address <<"\n";
            // cout << instr <<"\n";
            instr = stoi(num_read);
            if(mem_address==first_data_address) CURRENT_STATE=DATA_STATE; // achar o fim do section text, que será o STOP ou um JUMP, qual tiver o maior endereço
            
            if(CURRENT_STATE==OPCODE_STATE){
                imaginary_instr = *imaginary_instructions.get(instr);
                opcode=imaginary_instr;
                // cout << opcode <<"\n";
                translate_IA32 = *ia32_instructions.get(imaginary_instr);
                labels.add(mem_address, "BRANCH"+to_string(label_counter++));
                outputFileTemp << *labels.get(mem_address) << ":\n";

                if((5<=instr)&&(instr<=8)) branch_inst=true;

                if(imaginary_instr=="COPY"){
                    dbl_oper_inst=true;
                    copy_inst=true; // copy tem dois operands
                }
                if((imaginary_instr=="S_INPUT") || (imaginary_instr=="S_OUTPUT")) {
                    dbl_oper_inst=true;
                    s_IO_inst=true;
                }
                if(imaginary_instr=="STOP"){
                    stopFunction("",outputFileTemp); // stop nao tem argumento
                }else{
                    CURRENT_STATE=OPERAND_STATE;
                }
            }else if(CURRENT_STATE==OPERAND_STATE){
                if((dbl_oper_inst) || (NOT s_IO_inst)){ // só não entra pra segunda instrucao de S_IO
                    if((NOT branch_inst)&&(instr<first_data_address)) first_data_address=instr; // achei um referencia para acessar dados de uma linha menor da que eu achei q iniciava os dados -> atlz linha inicial de dados
                    if (labels.get(instr)==nullptr) labels.add(instr, "LABEL"+to_string(label_counter++));
                    if (used_labels.get(*labels.get(instr)+":")==nullptr) used_labels.add(*labels.get(instr)+":", instr);
                }
                branch_inst=false; // resetar a flag todo loop
                if(NOT dbl_oper_inst){                    
                    string* param0_ptr = labels.get(secondary_param);
                    string* param1_ptr = labels.get(instr);
                    if(param0_ptr!=nullptr){//nesse caso o opcode teve dois parametros e temos que ver ser o segundo é label(COPY) ou numero(S_I/O)
                        if(copy_inst){
                            translate_IA32(*param0_ptr +","+ *param1_ptr, outputFileTemp);
                        }else if(s_IO_inst){
                            translate_IA32(*param0_ptr +","+ to_string(instr), outputFileTemp);
                        }
                    }else translate_IA32(*param1_ptr, outputFileTemp); // aqui não acabou de passar(loop anterior) no else abaixo, pois secondary_param não existe 
                    secondary_param=-1;
                    copy_inst=false;s_IO_inst=false;
                    CURRENT_STATE=OPCODE_STATE;
                }else{
                    secondary_param=instr;
                    dbl_oper_inst=false;
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
        int buffer_size;
        string* buffer_name_ptr=nullptr;
        for (const auto &elem : *spaces.getData()) {
            if(labels.get(elem.first)==nullptr){
                buffer_size++;
            }else if(buffer_name_ptr!=nullptr){
                outputFile << "\t\t" << *buffer_name_ptr <<" resd "<<buffer_size<<"\n";
                buffer_name_ptr=labels.get(elem.first);
                buffer_size=1;
            }else{
                buffer_name_ptr=labels.get(elem.first);
                buffer_size=1;
            }    
        }
        outputFile << "\t\t" << *buffer_name_ptr <<" resd "<<buffer_size<<"\n"; // tem q fazer mais uma vez no final dos spaces

        outputFile<<"\nsection .data\n";
        outputFile<<"\t\tstr_lido db \"Foram lidos \", 0 \n";
        outputFile<<"\t\tlen_lido equ $-str_lido \n";

        outputFile<<"\t\tstr_bytes db \" bytes\", 0xA \n";
        outputFile<<"\t\tlen_bytes equ $-str_bytes\n";

        outputFile<<"\t\tstr_escrito db \"Foram escritos \", 0 \n";
        outputFile<<"\t\tlen_escrito equ $-str_escrito \n";
        outputFile<<"\t\tnwln db 0xA\n";

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

        writeStrInputFunction(outputFile);

        writeStrOutputFunction(outputFile);
        
        outputFile.close();

        // cout<<"\n\t----------------LABELS----------------\n";
        // labels.show();
        // cout<<"\n\t----------------USED_LABELS----------------\n";
        // used_labels.show();
        // cout<<"\n\t----------------spaces----------------\n";
        // spaces.show();

    }else {
        cerr << "Não foi possível abrir o arquivo." << "\n";
    }
    return 0;
}