#include "functions.hpp"



void init(ostream& output){
    output << "section .text \n\tglobal _start \n_start:\n";
}

void loadFunction(string param, ostream& output){
    output << "\tmov EAX, ["<<param<<"]\n";
}

void storeFunction(string param, ostream& output){
    output << "\tmov ["<<param<<"], EAX\n";
}

void subFunction(string param, ostream& output){
    output << "\tmov EBX,["<<param<<"]\n";
    output << "\tsub EAX, EBX\n";
}

void addFunction(string param, ostream& output){
    output << "\tmov EBX,["<<param<<"]\n";
    output << "\tadd EAX, EBX\n";
}

void copyFunction(string param, ostream& output){
    size_t c_index = param.find(',');
    string p1 = param.substr(0, c_index);
    string p2 = param.substr(c_index + 1);
    output << "\tmov ECX, ["<<p2<<"]\n";
    output << "\tmov ["<<p1<<"], ECX\n";
}

void jumpPosFunction(string param, ostream& output){
    output << "\tcmp EAX, 0\n";
    output << "\tjg "<<param<<"\n";
}

void jumpNegFunction(string param, ostream& output){
    output << "\tcmp EAX, 0\n";
    output << "\tjl "<<param<<"\n";
}

void jumpZeroFunction(string param, ostream& output){
    output << "\tcmp EAX, 0\n";
    output << "\tje "<<param<<"\n";
}

void jumpFunction(string param, ostream& output){
    output << "\tjmp "<<param<<"\n";
}

//https://stackoverflow.com/questions/3865828/simple-division-function-in-ia32-assembly
void divFunction(string param, ostream& output){
    output << "\tmov EBX, ["<<param<<"]\n";
    output << "\tcdq\n"; // estender sinal
    output << "\tidiv EBX\n";
}

//TODO:verificar overflow
void multFunction(string param, ostream& output){
    output << "\tmov EBX, ["<<param<<"]\n";
    output << "\timull EBX\n";
}

void stopFunction(string param, ostream& output){
    output << "\tmov EAX, 1\n";
    output << "\tmov EBX, 0\n";
    output << "\tint 80h\n";
}
