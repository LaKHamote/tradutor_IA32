#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <map>
#include <string>

using namespace std;

void init();

void loadFunction(string param){
    cout << "mov EBX,["<<param<<"]";
}

#endif 