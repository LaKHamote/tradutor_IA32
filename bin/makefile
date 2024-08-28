cpp_source = ../main.cpp
objs = ./input.o ./dataStructure.o ./functions.o

all:	tradutor.exe
	./tradutor.exe .$(path)

tradutor.exe:	$(objs) $(cpp_source)
	gcc -std=c++17 -m32 -Wall ./*.o $(cpp_source) -lstdc++ -o tradutor.exe 

input.o:	../input.asm
	nasm -f elf32 -o ./input.o ../input.asm

dataStructure.o:	../dataStructure.cpp
	gcc -std=c++17 -m32 -Wall -c ../dataStructure.cpp

functions.o:	../functions.cpp
	gcc -std=c++17 -m32 -Wall -c ../functions.cpp

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
	+$(MAKE)
	+$(MAKE) ia_32 path=$(path:.obj=.s)
	