#include "functions.hpp"



void init(ostream& output){
    output << "section .text \n\t\tglobal _start \n_start:\n";
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
    output << "\t\tmov ECX, ["<<p2<<"]\n";
    output << "\t\tmov ["<<p1<<"], ECX\n";
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
    output << "\t\timull EBX\n";
}

void stopFunction(string param, ostream& output){
    output << "\t\tmov EAX, 1\n";
    output << "\t\tmov EBX, 0\n";
    output << "\t\tint 80h\n";
}
