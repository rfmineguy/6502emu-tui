.PHONY: clean build
clean:
	rm *.o || true
	rm *.map || true
	rm *.dbg || true
	rm *.lst || true
	rm test || true

build: test.s
	ca65 test.s -l test.lst
	ld65 -o test test.o --config test.cfg -m test.map --dbgfile test.dbg
