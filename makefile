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