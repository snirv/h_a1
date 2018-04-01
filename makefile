all: calc

calc: calc.o asm.o
	gcc -g -Wall -o calc calc.o asm.o

calc.o: calc.c
	gcc -g -Wall -c -o calc.o calc.c 

 
asm.o: asm.s
	nasm -g -f elf64 -w+all -o asm.o asm.s

clean: 
	rm -f *.o calc