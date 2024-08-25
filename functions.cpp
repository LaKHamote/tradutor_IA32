#include "functions.hpp"


void init(){
    cout << "section .text \n\tglobal _start \n_start:\n";
}

void loadFunction(string param){
    cout << "\tmovl EAX, ["<<param<<"]\n";
}

void storeFunction(string param){
    cout << "\tmovl ["<<param<<"], EAX\n";
}

void subFunction(string param){
    cout << "\tmovl EBX,["<<param<<"]\n";
    cout << "\tsub EAX, EBX\n";
}

void addFunction(string param){
    cout << "\tmovl EBX,["<<param<<"]\n";
    cout << "\tadd EAX, EBX\n";
}

void copyFunction(string param){
    size_t c_index = param.find(',');
    string p1 = param.substr(0, c_index);
    string p2 = param.substr(c_index + 1);
    cout << "\tmovl ECX, ["<<p2<<"]\n";
    cout << "\tmovl ["<<p1<<"], ECX\n";
}

void jumpPosFunction(string param){
    cout << "\tcmp EAX, 0\n";
    cout << "\tjg "<<param<<"\n";
}

void jumpNegFunction(string param){
    cout << "\tcmp EAX, 0\n";
    cout << "\tjl "<<param<<"\n";
}

void jumpZeroFunction(string param){
    cout << "\tcmp EAX, 0\n";
    cout << "\tje "<<param<<"\n";
}

void jumpFunction(string param){
    cout << "\tjmp "<<param<<"\n";
}

//https://stackoverflow.com/questions/3865828/simple-division-function-in-ia32-assembly
void divFunction(string param){
    cout << "\tmovl EBX, ["<<param<<"]\n";
    cout << "\tcdq\n"; // estender sinal
    cout << "\tidiv EBX\n";
}

//TODO:verificar overflow
void multFunction(string param){
    cout << "\tmovl EBX, ["<<param<<"]\n";
    cout << "\timull EBX\n";
}

void stopFunction(string param){
    cout << "\tmov EAX, 1";
    cout << "\tmov EBX, 0";
    cout << "\tint 0x80";
}
