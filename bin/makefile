cpp_source = ../main.cpp

all:	tradutor.exe
	./tradutor.exe

tradutor.exe:	dataStructure.o $(cpp_source)
	gcc -std=c++17 -Wall ./*.o $(cpp_source) -lstdc++ -o tradutor.exe 

dataStructure.o:	../dataStructure.cpp
	gcc -std=c++17 -Wall -c ../dataStructure.cpp