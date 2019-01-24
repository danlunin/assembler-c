CC=gcc
CXX=gcc+
LD=~/bin/clang++
WARN_OPTS=-Wall -Werror -pedantic

CFLAGS=$(WARN_OPTS) $(INCLUDES) $(FLAGS) -std=c11 -g

DEPS = mainFunctions.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


all: asm vm	check

vm: vm.o mainFunctions.o mainFunctions.h
	$(CC) -o vm mainFunctions.o vm.o

asm: asm.o mainFunctions.o mainFunctions.h
	$(CC) -o asm mainFunctions.o asm.o
	
check: check.o mainFunctions.o mainFunctions.h 
	$(CC) -o check mainFunctions.o check.o