all: roundtrip
.PHONY: all

clean:
	rm -f *.o roundtrip

roundtrip: roundtrip.o pgmio.o quad.o
	gcc -g $^ -o roundtrip -lm

roundtrip.o : pgmio.h quad.h

pgmio.o : pgmio.h

quad.o : quad.h pgmio.h

%.o : %.c
	gcc -g -c $<
