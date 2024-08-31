#include "functions.hpp"



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
    string p1 = param.substr(0, c_index);
    string p2 = param.substr(c_index + 1);
    output << "\t\tmov ECX, ["<<p1<<"]\n";
    output << "\t\tmov ["<<p2<<"], ECX\n";
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
}

void stopFunction(string param, ostream& output){
    output << "\t\tmov EAX, 1\n";
    output << "\t\tmov EBX, 0\n";
    output << "\t\tint 80h\n";
}

void inputFunction(string param, ostream& output){
    output << "\t\tcall input_function ; Input será carregado em EDX, numero de bytes lidos em EBX e EAX\n";
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

    output << "\t\tcall output_function ; Mostra bytes lidos no console\n";
    
}

void outputFunction(string param, ostream& output){
    output << "\t\tpush EBX ; joga valor atual de EBX na stack\n";
    output << "\t\tpush EAX ; joga valor atual de EAX na stack\n";
    output << "\t\tmov EBX, " "["<<param<<"]" << "\n";
    output << "\t\tcall output_function ; Mostra label que foi colocado em EBX\n";
    
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
    output << "\t\tcall output_function ; Mostra bytes escritos em EAX\n";
    output << "\t\tpop EAX ; volta valor antigo de EAX\n";
    output << "\t\tpop EBX ; volta valor antigo de EBX\n";
}
