all:
	+$(MAKE) -C bin

clean:
	rm -f ./bin/*.o
	rm -f ./bin/*.exe
	rm -f ./bin/*.out

clean_examples:
	rm -f ./examples/*.tmp
	rm -f ./examples/*.s

ia_32:
	+$(MAKE) -C bin  ia_32

run:
	+$(MAKE) -C bin  run

test:	$(path)
	nasm -f elf $(path) -o $(path:.tmp=.o)
	ld -m elf_i386 -o $(path:.tmp=.exe) $(path:.tmp=.o)
	@echo "--------Running .exe file...--------"
	./$(path:.tmp=.exe)
	@echo "\n--------.exe file has finished executing.--------"
	rm -f ./$(path:.tmp=.o)
	rm -f ./$(path:.tmp=.exe)