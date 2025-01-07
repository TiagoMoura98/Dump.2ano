main: main.o func.o asm.o
	gcc -g main.o func.o asm.o -z noexecstack -o main

main.o: main.c asm.h
	gcc -g -Wall -Wextra -fanalyzer -c main.c -o main.o

func.o: func.s
	gcc -g -Wall -Wextra -fanalyzer -c func.s -o func.o	

asm.o: asm.s
	gcc -g -Wall -Wextra -fanalyzer -c asm.s -o asm.o	

clean:
	rm -f *.o main

run: main
	./main