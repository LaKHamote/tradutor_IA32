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
    {"INPUT", inputFunction},
    {"OUTPUT", outputFunction},
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
        outputFile<<"\t\tstr_lido db \"Bytes lidos: \", 0x0A \n";
        outputFile<<"\t\tlen_lido equ $-str_lido \n";
        outputFile<<"\t\tstr_escrito db \"Bytes escritos: \", 0X0A \n";
        outputFile<<"\t\tlen_escrito equ $-str_escrito \n";

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
        //------------------------------------------------------adiciona--funcao--de--input--------------------------------------------
        outputFile << "input_function:\n";
        outputFile << "    xor ebx, ebx       ; ebx conta quantos bytes foram lidos, começa em zero\n";
        outputFile << "    xor edx, edx       ; edx será o acumulador para o número final, começa em zero\n";
        outputFile << "    call le_char       ; Chama a função que lê um caractere do usuário\n\n";

        outputFile << "    cmp cl, 0x0A       ; Compara o caractere lido com '\\n' (0x0A)\n";
        outputFile << "    je fim             ; Se o caractere lido for '\\n', pula para o fim para encerrar o programa\n";
        outputFile << "    cmp cl, '-'        ; Compara o caractere lido com '-' para verificar se é negativo\n";
        outputFile << "    je loop_negativo   ; Se for '-', pula para o loop de números negativos\n";
        outputFile << "    jmp positivo       ; Caso contrário, pula para o loop de números positivos\n\n";

        outputFile << "loop_positivo:\n";
        outputFile << "    call le_char       ; Chama a função para ler o próximo caractere\n";
        outputFile << "    cmp cl, 0x0A       ; Compara o caractere lido com '\\n' (0x0A)\n";
        outputFile << "    je fim             ; Se o caractere lido for '\\n', pula para o fim para encerrar o programa\n";
        outputFile << "    jmp positivo       ; Continua o processo de leitura e conversão de números positivos\n\n";

        outputFile << "loop_negativo:\n";
        outputFile << "    call le_char       ; Chama a função para ler o próximo caractere\n";
        outputFile << "    cmp cl, 0x0A       ; Compara o caractere lido com '\\n' (0x0A)\n";
        outputFile << "    je fim             ; Se o caractere lido for '\\n', pula para o fim para encerrar o programa\n";
        outputFile << "    jmp negativo       ; Continua o processo de leitura e conversão de números negativos\n\n";

        outputFile << "positivo:\n";
        outputFile << "    sub cl, '0'        ; Converte o caractere numérico em seu valor decimal\n";
        outputFile << "    movzx eax, cl      ; Move o valor decimal para eax\n";
        outputFile << "    imul edx, 10       ; Multiplica o acumulador (edx) por 10 para ajustar a posição do dígito\n";
        outputFile << "    add edx, eax       ; Adiciona o valor atual de eax ao acumulador edx\n";
        outputFile << "    jmp loop_positivo  ; Volta para o loop de leitura de números positivos\n\n";

        outputFile << "negativo:\n";
        outputFile << "    sub cl, '0'        ; Converte o caractere numérico em seu valor decimal\n";
        outputFile << "    movzx eax, cl      ; Move o valor decimal para eax\n";
        outputFile << "    imul edx, 10       ; Multiplica o acumulador (edx) por 10 para ajustar a posição do dígito\n";
        outputFile << "    sub edx, eax       ; Subtrai o valor atual de eax do acumulador edx\n";
        outputFile << "    jmp loop_negativo  ; Volta para o loop de leitura de números negativos\n\n";

        outputFile << "fim:\n";
        outputFile << "    ; O numero final está em edx!\n";
        outputFile << "    mov eax, ebx       ; Coloca o contador de bytes em eax tambem\n";
        outputFile << "    ret\n\n";

        outputFile << "le_char:\n";
        outputFile << "    ; Função para ler um caractere inserido pelo usuário\n";
        outputFile << "    ; Recebe o caractere do console e o retorna em cl\n";
        outputFile << "    ; Se o caractere lido for '\\n', encerra o programa\n\n";

        outputFile << "    add ebx, 1\n\n";

        outputFile << "    push eax               ; Salva o valor atual de eax na pilha\n";
        outputFile << "    push ebx               ; Salva o valor atual de ebx na pilha\n";
        outputFile << "    push edx               ; Salva o valor atual de edx na pilha\n\n";

        outputFile << "    sub esp, 1             ; Reserva um byte na pilha\n";
        outputFile << "    mov eax, 3             ; syscall número 3: sys_read (chamada para leitura)\n";
        outputFile << "    mov ebx, 0             ; Ler da entrada padrão (stdin)\n";
        outputFile << "    lea ecx, [esp]         ; Aponta ecx para o local na pilha onde o byte será armazenado\n";
        outputFile << "    mov edx, 1             ; Define o tamanho do buffer como 1 byte\n";
        outputFile << "    int 0x80               ; Chamada de sistema para ler o caractere\n";
        outputFile << "    mov cl, byte [esp]     ; Coloca o byte lido em cl\n";
        outputFile << "    add esp, 1             ; Restaura o ponteiro da pilha\n";
        outputFile << "    pop edx                ; Restaura o valor original de edx da pilha\n";
        outputFile << "    pop ebx                ; Restaura o valor original de ebx da pilha\n";
        outputFile << "    pop eax                ; Restaura o valor original de eax da pilha\n\n";

        outputFile << "    ret                    ; Retorna da função le_char\n";


        //------------------------------------------------------adiciona--funcao--de--output-------------------------------------------
        outputFile << "output_function:\n";
        outputFile << "; Lê o numero em EBX e imprime\n";
        outputFile << "    xor ecx, ecx          ; Zera ECX, usado para contar o número de dígitos (inicialmente 0)\n";
        outputFile << "    cmp ebx, 0            ; Compara o valor de EBX com 0 para verificar se é negativo\n";
        outputFile << "    jge n_positivo        ; Se EBX for maior ou igual a 0 (não negativo), salta para o rótulo n_positivo\n\n";
        outputFile << "    mov eax, '-'          ; Se o número for negativo, coloca o caractere '-' em EAX\n";
        outputFile << "    call escreve_char     ; Chama a função escreve_char para imprimir o caractere '-'\n";
        outputFile << "    neg ebx               ; Torna EBX positivo (se estava negativo)\n";
        outputFile << "n_positivo:\n";
        outputFile << "    mov eax, ebx          ; Move o valor absoluto de EBX para EAX (preparando para conversão de dígitos)\n\n";
        outputFile << "convert_loop:\n";
        outputFile << "    xor edx, edx          ; Zera EDX, necessário para a operação de divisão\n";
        outputFile << "    mov ebx, 10           ; Carrega o divisor 10 em EBX para a divisão\n";
        outputFile << "    div ebx               ; Divide EAX por EBX (10), resultado em EAX (quociente) e EDX (resto)\n";
        outputFile << "    add ecx, 1            ; Incrementa o contador de dígitos (ECX) em 1\n";
        outputFile << "    add edx, '0'          ; Converte o resto (dígito) em seu valor ASCII\n";
        outputFile << "    push edx              ; Empilha o caractere ASCII na pilha\n";
        outputFile << "    cmp eax, 0            ; Compara o quociente (EAX) com 0\n";
        outputFile << "    jne convert_loop      ; Se o quociente não for 0, continua o loop para processar o próximo dígito\n\n";
        outputFile << "    mov ebx, ecx          ; Guarda em ebx os bytes escritos\n\n";
        outputFile << "    add ebx, 1            ; Guarda em ebx os bytes escritos\n\n";
        outputFile << "imprime_pilha:\n";
        outputFile << "    cmp ecx, 0            ; Compara o contador de dígitos (ECX) com 0\n";
        outputFile << "    je end                ; Se ECX for 0 (não há mais dígitos para imprimir), salta para o rótulo end\n";
        outputFile << "    pop eax               ; Remove o próximo caractere da pilha e coloca em EAX\n";
        outputFile << "    call escreve_char     ; Chama a função escreve_char para imprimir o caractere\n";
        outputFile << "    sub ecx, 1            ; Decrementa o contador de dígitos (ECX) em 1\n";
        outputFile << "    add eax, 1            ; incrementa o contador de bytes (EAX) em 1\n";
        outputFile << "    jmp imprime_pilha     ; Volta para o início do rótulo imprime_pilha para imprimir o próximo caractere\n\n";
        outputFile << "escreve_char:\n";
        outputFile << "    push ecx\n";
        outputFile << "    push ebx\n";
        outputFile << "    push edx\n";
        outputFile << "    push eax\n\n";
        outputFile << "    mov eax, 4            ; Define o número da syscall para sys_write (4)\n";
        outputFile << "    mov ebx, 1            ; Define o descritor de arquivo para stdout (1)\n";
        outputFile << "    mov ecx, esp          ; Define o ponteiro de dados (ECX) como o topo da pilha (contém o caractere a ser escrito)\n";
        outputFile << "    mov edx, 1            ; Define o número de bytes a serem escritos (1 byte)\n";
        outputFile << "    int 0x80              ; Chama o kernel para executar a syscall sys_write\n\n";
        outputFile << "    pop eax\n";
        outputFile << "    pop edx\n";
        outputFile << "    pop ebx\n";
        outputFile << "    pop ecx\n";
        outputFile << "    ret\n\n";
        outputFile << "end:\n";
        outputFile << "    push eax\n";
        outputFile << "    mov eax, 0x0A        ; Carrega o valor do caractere de nova linha (Line Feed) em EAX\n";
        outputFile << "    call escreve_char    ; Chama a função escreve_char para imprimir a nova linha\n";
        outputFile << "    pop eax              ; Preserva o valor de bytes escritos em eax\n";           
        outputFile << "    mov eax,ebx          ; Guarda os bytes escritos + endline \n";
        outputFile << "    ret                  ; Retorna da função output_function\n";

    

        outputFile.close();

    }else {
        cerr << "Não foi possível abrir o arquivo." << "\n";
    }
    return 0;
}