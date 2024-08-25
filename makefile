all:
	+$(MAKE) -C bin

clean:
	rm -f ./bin/*.o
	rm -f ./bin/*.exe
	rm -f ./bin/*.out

ia_32:
	nasm -f elf program.asm -o program.o
	ld -m elf_i386 -o program program.o
	./program