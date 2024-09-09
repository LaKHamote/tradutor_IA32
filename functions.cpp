#include "functions.hpp"
#include <fstream>
#include <sstream>


void init(ostream& output){
    output << "\nsection .text \n\t\tglobal _start \n_start:\n";
}

void loadFunction(string param, ostream& output){
    output << "\t\tmov EAX, ["<<param<<"]\n";
}

void storeFunction(string param, ostream& output){
    output << "\t\tmov ["<<param<<"], EAX\n";
}

void subFunction(string param, ostream& output){
    output << "\t\tmov EBX,["<<param<<"]\n";
    output << "\t\tsub EAX, EBX\n";
}

void addFunction(string param, ostream& output){
    output << "\t\tmov EBX,["<<param<<"]\n";
    output << "\t\tadd EAX, EBX\n";
}

void copyFunction(string param, ostream& output){
    size_t c_index = param.find(',');
    string param1 = param.substr(0, c_index);
    string param2 = param.substr(c_index + 1);
    output << "\t\tmov ECX, ["<<param1<<"]\n";
    output << "\t\tmov ["<<param2<<"], ECX\n";
}

void jumpPosFunction(string param, ostream& output){
    output << "\t\tcmp EAX, 0\n";
    output << "\t\tjg "<<param<<"\n";
}

void jumpNegFunction(string param, ostream& output){
    output << "\t\tcmp EAX, 0\n";
    output << "\t\tjl "<<param<<"\n";
}

void jumpZeroFunction(string param, ostream& output){
    output << "\t\tcmp EAX, 0\n";
    output << "\t\tje "<<param<<"\n";
}

void jumpFunction(string param, ostream& output){
    output << "\t\tjmp "<<param<<"\n";
}

//https://stackoverflow.com/questions/3865828/simple-division-function-in-ia32-assembly
void divFunction(string param, ostream& output){
    output << "\t\tmov EBX, ["<<param<<"]\n";
    output << "\t\tcdq\n"; // estender sinal
    output << "\t\tidiv EBX\n";
}

//TODO:verificar overflow
void multFunction(string param, ostream& output){
    output << "\t\tmov EBX, ["<<param<<"]\n";
    output << "\t\timul EBX\n";
    output << "\t\tjo overflow_detected\n";
}

void stopFunction(string param, ostream& output){
    output << "\t\tmov EAX, 1\n";
    output << "\t\tmov EBX, 0\n";
    output << "\t\tint 80h\n";
}

void callinputFunction(string param, ostream& output){
    output << "; Input não recebe argumentos, logo não há push para a stack!.\n";
    output << "\t\tcall input_function ; Input será carregado em EDX, numero de bytes lidos em EBX e EAX.\n";
    output << "\t\tmov " << "["<<param<<"], " << "EDX" << "\n";

    
    output << "\t\tpush EBX ; joga valor atual de EBX na stack\n";
    output << "\t\tpush ECX ; joga valor atual de ECX na stack\n";
    output << "\t\tpush EDX ; joga valor atual de EDX na stack\n";
    output << "\t\tpush EAX ; joga valor atual de EAX na stack\n";

    output << "\t\tmov eax, 4\n";
    output << "\t\tmov ebx, 1\n";
    output << "\t\tmov ecx, str_lido\n";
    output << "\t\tmov edx, len_lido\n";
    output << "\t\tint 0x80\n";

    output << "\t\tpop EAX ; volta valor antigo de EAX\n";
    output << "\t\tpop EDX ; volta valor antigo de EDX\n";
    output << "\t\tpop ECX ; volta valor antigo de ECX\n";
    output << "\t\tpop EBX ; volta valor antigo de EBX\n";

    output << "\t\tpush EBX ; joga valor atual de EBX na stack, para argumento de output\n";
    output << "\t\tcall output_function ; Mostra bytes lidos no console\n";

    output << "\t\tmov eax, 4\n";
    output << "\t\tmov ebx, 1\n";
    output << "\t\tmov ecx, str_bytes\n";
    output << "\t\tmov edx, len_bytes\n";
    output << "\t\tint 0x80\n";
}

void calloutputFunction(string param, ostream& output){
    output << "\t\tmov EBX, " "["<<param<<"]" << "\n";
    output << "\t\tpush EBX" << "\n";
    output << "\t\tcall output_function ; Mostra label que foi colocado em EBX\n";

    output << "\t\tpush EAX\n";
    output << "\t\tmov EAX, 0x0A        ; Carrega o valor do caractere de nova linha (Line Feed) em EAX\n";
    output << "\t\tcall escreve_char    ; Chama a função escreve_char para imprimir a nova linha\n";
    output << "\t\tpop EAX              ; Preserva o valor de bytes escritos em eax\n"; 
    
    output << "\t\tpush EBX ; joga valor atual de EBX na stack\n";
    output << "\t\tpush ECX ; joga valor atual de ECX na stack\n";
    output << "\t\tpush EDX ; joga valor atual de EDX na stack\n";
    output << "\t\tpush EAX ; joga valor atual de EAX na stack\n";

    output << "\t\tmov eax, 4\n";
    output << "\t\tmov ebx, 1\n";
    output << "\t\tmov ecx, str_escrito\n";
    output << "\t\tmov edx, len_escrito\n";
    output << "\t\tint 0x80\n";

    output << "\t\tpop EAX ; volta valor antigo de EAX\n";
    output << "\t\tpop EDX ; volta valor antigo de EDX\n";
    output << "\t\tpop ECX ; volta valor antigo de ECX\n";
    output << "\t\tpop EBX ; volta valor antigo de EBX\n";
    output << "\t\t push EBX ; volta valor antigo de EBX\n";
    output << "\t\tcall output_function ; Mostra bytes escritos em EAX\n";

    output << "\t\tmov eax, 4\n";
    output << "\t\tmov ebx, 1\n";
    output << "\t\tmov ecx, str_bytes\n";
    output << "\t\tmov edx, len_bytes\n";
    output << "\t\tint 0x80\n";

}


void writeinputFunction(ostream& output){
    output << "\n\ninput_function:\n";
    output << "    \txor ebx, ebx       ; ebx conta quantos bytes foram lidos, começa em zero\n";
    output << "    \txor edx, edx       ; edx será o acumulador para o número final, começa em zero\n";
    output << "    \tcall le_char       ; Chama a função que lê um caractere do usuário\n\n";

    output << "    \tcmp cl, 0x0A       ; Compara o caractere lido com '\\n' (0x0A)\n";
    output << "    \tje fim             ; Se o caractere lido for '\\n', pula para o fim para encerrar o programa\n";
    output << "    \tcmp cl, '-'        ; Compara o caractere lido com '-' para verificar se é negativo\n";
    output << "    \tje loop_negativo   ; Se for '-', pula para o loop de números negativos\n";
    output << "    \tjmp positivo       ; Caso contrário, pula para o loop de números positivos\n\n";

    output << "loop_positivo:\n";
    output << "    \tcall le_char       ; Chama a função para ler o próximo caractere\n";
    output << "    \tcmp cl, 0x0A       ; Compara o caractere lido com '\\n' (0x0A)\n";
    output << "    \tje fim             ; Se o caractere lido for '\\n', pula para o fim para encerrar o programa\n";
    output << "    \tjmp positivo       ; Continua o processo de leitura e conversão de números positivos\n\n";

    output << "loop_negativo:\n";
    output << "    \tcall le_char       ; Chama a função para ler o próximo caractere\n";
    output << "    \tcmp cl, 0x0A       ; Compara o caractere lido com '\\n' (0x0A)\n";
    output << "    \tje fim             ; Se o caractere lido for '\\n', pula para o fim para encerrar o programa\n";
    output << "    \tjmp negativo       ; Continua o processo de leitura e conversão de números negativos\n\n";

    output << "positivo:\n";
    output << "    \tsub cl, '0'        ; Converte o caractere numérico em seu valor decimal\n";
    output << "    \tmovzx eax, cl      ; Move o valor decimal para eax\n";
    output << "    \timul edx, 10       ; Multiplica o acumulador (edx) por 10 para ajustar a posição do dígito\n";
    output << "    \tadd edx, eax       ; Adiciona o valor atual de eax ao acumulador edx\n";
    output << "    \tjmp loop_positivo  ; Volta para o loop de leitura de números positivos\n\n";

    output << "negativo:\n";
    output << "    \tsub cl, '0'        ; Converte o caractere numérico em seu valor decimal\n";
    output << "    \tmovzx eax, cl      ; Move o valor decimal para eax\n";
    output << "    \timul edx, 10       ; Multiplica o acumulador (edx) por 10 para ajustar a posição do dígito\n";
    output << "    \tsub edx, eax       ; Subtrai o valor atual de eax do acumulador edx\n";
    output << "    \tjmp loop_negativo  ; Volta para o loop de leitura de números negativos\n\n";

    output << "fim:\n";
    output << "    \t; O numero final está em edx!\n";
    output << "    \tsub ebx, 1       ; Coloca o contador de bytes em eax tambem\n";
    output << "    \tmov eax, ebx       ; Coloca o contador de bytes em eax tambem\n";
    output << "    \tret\n\n";

    output << "le_char:\n";
    output << "    \t; Função para ler um caractere inserido pelo usuário\n";
    output << "    \t; Recebe o caractere do console e o retorna em cl\n";
    output << "    \t; Se o caractere lido for '\\n', encerra o programa\n\n";

    output << "    \tadd ebx, 1\n\n";

    output << "    \tpush eax               ; Salva o valor atual de eax na pilha\n";
    output << "    \tpush ebx               ; Salva o valor atual de ebx na pilha\n";
    output << "    \tpush edx               ; Salva o valor atual de edx na pilha\n\n";

    output << "    \tsub esp, 1             ; Reserva um byte na pilha\n";
    output << "    \tmov eax, 3             ; syscall número 3: sys_read (chamada para leitura)\n";
    output << "    \tmov ebx, 0             ; Ler da entrada padrão (stdin)\n";
    output << "    \tlea ecx, [esp]         ; Aponta ecx para o local na pilha onde o byte será armazenado\n";
    output << "    \tmov edx, 1             ; Define o tamanho do buffer como 1 byte\n";
    output << "    \tint 0x80               ; Chamada de sistema para ler o caractere\n";
    output << "    \tmov cl, byte [esp]     ; Coloca o byte lido em cl\n";
    output << "    \tadd esp, 1             ; Restaura o ponteiro da pilha\n";
    output << "    \tpop edx                ; Restaura o valor original de edx da pilha\n";
    output << "    \tpop ebx                ; Restaura o valor original de ebx da pilha\n";
    output << "    \tpop eax                ; Restaura o valor original de eax da pilha\n\n";

    output << "    \tret                    ; Retorna da função le_char\n";
}

void writeoutputFunction(ostream& output){
    output << "\n\noutput_function:\n";
    output << "; Lê o numero passado em [ebp+8] e imprime\n";
    output << "    \tenter 0,0             ; Entra no contexto de pilha em que [ebp+8] eh o argumento passado\n";
    output << "    \tmov ebx, [ebp+8]      ; Pega o argumento passado por PILHA e poem em ebx\n";
    output << "    \txor ecx, ecx          ; Zera ECX, usado para contar o número de dígitos (inicialmente 0)\n";
    output << "    \tcmp ebx, 0            ; Compara o valor de EBX com 0 para verificar se é negativo\n";
    output << "    \tjge n_positivo        ; Se EBX for maior ou igual a 0 (não negativo), salta para o rótulo n_positivo\n\n";
    output << "    \tmov eax, '-'          ; Se o número for negativo, coloca o caractere '-' em EAX\n";
    output << "    \tcall escreve_char     ; Chama a função escreve_char para imprimir o caractere '-'\n";
    output << "    \tneg ebx               ; Torna EBX positivo (se estava negativo)\n";
    output << "n_positivo:\n";
    output << "    \tmov eax, ebx          ; Move o valor absoluto de EBX para EAX (preparando para conversão de dígitos)\n\n";
    output << "convert_loop:\n";
    output << "    \txor edx, edx          ; Zera EDX, necessário para a operação de divisão\n";
    output << "    \tmov ebx, 10           ; Carrega o divisor 10 em EBX para a divisão\n";
    output << "    \tdiv ebx               ; Divide EAX por EBX (10), resultado em EAX (quociente) e EDX (resto)\n";
    output << "    \tadd ecx, 1            ; Incrementa o contador de dígitos (ECX) em 1\n";
    output << "    \tadd edx, '0'          ; Converte o resto (dígito) em seu valor ASCII\n";
    output << "    \tpush edx              ; Empilha o caractere ASCII na pilha\n";
    output << "    \tcmp eax, 0            ; Compara o quociente (EAX) com 0\n";
    output << "    \tjne convert_loop      ; Se o quociente não for 0, continua o loop para processar o próximo dígito\n\n";
    output << "    \tmov ebx, ecx          ; Guarda em ebx os bytes escritos\n\n";
    output << "imprime_pilha:\n";
    output << "    \tcmp ecx, 0            ; Compara o contador de dígitos (ECX) com 0\n";
    output << "    \tje end                ; Se ECX for 0 (não há mais dígitos para imprimir), salta para o rótulo end\n";
    output << "    \tpop eax               ; Remove o próximo caractere da pilha e coloca em EAX\n";
    output << "    \tcall escreve_char     ; Chama a função escreve_char para imprimir o caractere\n";
    output << "    \tsub ecx, 1            ; Decrementa o contador de dígitos (ECX) em 1\n";
    output << "    \tadd eax, 1            ; incrementa o contador de bytes (EAX) em 1\n";
    output << "    \tjmp imprime_pilha     ; Volta para o início do rótulo imprime_pilha para imprimir o próximo caractere\n\n";
    output << "escreve_char:\n";
    output << "    \tpush ecx\n";
    output << "    \tpush ebx\n";
    output << "    \tpush edx\n";
    output << "    \tpush eax\n\n";
    output << "    \tmov eax, 4            ; Define o número da syscall para sys_write (4)\n";
    output << "    \tmov ebx, 1            ; Define o descritor de arquivo para stdout (1)\n";
    output << "    \tmov ecx, esp          ; Define o ponteiro de dados (ECX) como o topo da pilha (contém o caractere a ser escrito)\n";
    output << "    \tmov edx, 1            ; Define o número de bytes a serem escritos (1 byte)\n";
    output << "    \tint 0x80              ; Chama o kernel para executar a syscall sys_write\n\n";
    output << "    \tpop eax\n";
    output << "    \tpop edx\n";
    output << "    \tpop ebx\n";
    output << "    \tpop ecx\n";
    output << "    \tret\n\n";
    output << "end:\n";          
    output << "    \tmov eax,ebx           ; Guarda os bytes escritos + endline \n";
    output << "    \tleave                 ; Sai do contexto de pilha criado para receber o argumento \n";
    output << "    \tret 4                 ; Retorna da função output_function\n";
}

void callStrInputFunction(string param, ostream& output){
    size_t c_index = param.find(',');
    string param1 = param.substr(0, c_index);
    string param2 = param.substr(c_index + 1);
    output << "\t\tpush "<<param1<<"            ;ponteiro para onde quardar\n";
    output << "\t\tpush "<<param2<<"              ;numero de bytes pra quardar\n";
    output << "\t\tcall s_input_function\n";
}

void callStrOutputFunction(string param, ostream& output){
    size_t c_index = param.find(',');
    string param1 = param.substr(0, c_index);
    string param2 = param.substr(c_index + 1);
    output << "\t\tpush "<<param1<<"            ;ponteiro para do que escrever\n";
    output << "\t\tpush "<<param2<<"              ;numero de bytes pra escrever\n";
    output << "\t\tcall s_output_function\n";
}

void writeStrInputFunction(ostream& output){
    ifstream file("../bin/s_input_function.txt");
    if (!file.is_open()) {
        throw invalid_argument("\n\n\tArquivo nao encontrado\n");
    }
    output << file.rdbuf();
    file.close();
}

void writeStrOutputFunction(ostream& output){
    ifstream file("../bin/s_output_function.txt");
    if (!file.is_open()) {
        throw invalid_argument("\n\n\tArquivo nao encontrado\n");
    }
    output << file.rdbuf();
    file.close();
}