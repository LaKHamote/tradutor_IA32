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
	nasm -f elf $(path) -o $(path:.s=.o)
	ld -m elf_i386 $(path:.s=.o) -o $(path:.s=.exe)
	@echo "--------Running .exe file...--------"
	./$(path:.s=.exe)
	@echo "\n--------.exe file has finished executing.--------"
	rm -f ./$(path:.s=.o)
	rm -f ./$(path:.s=.tmp)
	rm -f ./$(path:.s=.exe)