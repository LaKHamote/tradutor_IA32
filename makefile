all:
	+$(MAKE) -C bin

clean:
	rm -f ./bin/*.o
	rm -f ./bin/*.exe
	rm -f ./bin/*.out

clean_examples:
	rm -f ./examples/*.o
	rm -f ./examples/*.exe
	rm -f ./examples/*.temp
	rm -f ./examples/*.out
	rm -f ./examples/*.s

ia_32:
	+$(MAKE) -C bin  ia_32

run:
	+$(MAKE) -C bin  run

input:	input_S.asm
	nasm -f elf input_S.asm -o input_S.o
	ld -m elf_i386 -o input_S.exe input_S.o
	@echo "--------Running .exe file...--------"
	./input_S.exe
	@echo "\n--------.exe file has finished executing.--------"
	rm -f ./input_S.o
	rm -f ./input_S.exe