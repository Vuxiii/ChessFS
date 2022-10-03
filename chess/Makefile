CC=gcc
CFLAGS=-g -Wall -Wextra -fsanitize=address -fsanitize=leak -fsanitize=undefined
LDFLAGS=-fsanitize=address -fsanitize=leak -fsanitize=undefined
LDFLAGS += -lm

all: skak

# moves.o: chess.o datastructures.o

skak: chess.o moves.o datastructures.o
	${CC} ${LDFLAGS} -o $@ $^


.PHONY: clean
clean:
	rm -f skak  *.o
