cpp_source = ../main.cpp
objs = ./dataStructure.o ./functions.o

all:	tradutor.exe
	./tradutor.exe

tradutor.exe:	$(objs) $(cpp_source)
	gcc -std=c++17 -Wall ./*.o $(cpp_source) -lstdc++ -o tradutor.exe 

dataStructure.o:	../dataStructure.cpp
	gcc -std=c++17 -Wall -c ../dataStructure.cpp

functions.o:	../functions.cpp
	gcc -std=c++17 -Wall -c ../functions.cpp