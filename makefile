all:
	+$(MAKE) -C bin

clean:
	rm -f ./bin/*.o
	rm -f ./bin/*.exe
	rm -f ./bin/*.out