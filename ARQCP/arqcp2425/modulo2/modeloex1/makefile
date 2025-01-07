main: main.o asm.o 
	gcc -g main.o asm.o -z noexecstack -o main

main.o: main.c asm.h
	gcc -g -Wall -Wextra -fanalyzer -c main.c -o main.o

asm.o: asm.s
	gcc -g -Wall -Wextra -fanalyzer -c asm.s -o asm.o	

clean:
	rm -f *.o main

run: main
	./main