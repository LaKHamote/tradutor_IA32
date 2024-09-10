cpp_source = ../main.cpp
objs = ./dataStructure.o ./functions.o

all:	$(objs) $(cpp_source)
	gcc -std=c++17 -Wall ./*.o $(cpp_source) -lstdc++ -o ../tradutor 

tradutor.exe:	$(objs) $(cpp_source)
	gcc -std=c++17 -Wall ./*.o $(cpp_source) -lstdc++ -o tradutor.exe

dataStructure.o:	../dataStructure.cpp
	gcc -std=c++17 -Wall -c ../dataStructure.cpp

functions.o:	../functions.cpp
	gcc -std=c++17 -Wall -c ../functions.cpp

asm:	tradutor.exe
	./tradutor.exe .$(path)

ia_32:
	nasm -f elf .$(path) -o .$(path:.s=.o)
	ld -m elf_i386 -o .$(path:.s=.exe) .$(path:.s=.o)
	rm -f ../$(path:.s=.o)
	rm -f ../$(path:.s=.tmp)
	@echo "--------Running .exe file...--------"
	../$(path:.s=.exe)
	@echo "\n--------.exe file has finished executing.--------"
	rm -f ../$(path:.s=.exe)

run:
	+$(MAKE)	asm
	+$(MAKE) 	ia_32 path=$(path:.obj=.s)
	