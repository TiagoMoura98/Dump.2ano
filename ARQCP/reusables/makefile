# Variables
INCLUDES = 
SOURCES = file.c
OBJFILES = file.o
EXEC = prog
# Suffix rules
.SUFFIXES : .c .o
# How to build an object .o from a code file .c ; $
.c.o:
	gcc -g -Wall -Wextra -fanalyzer -c $<
# How to build the executable from object files
${EXEC}: ${OBJFILES}
	gcc ${OBJFILES} -o ${EXEC}
# Run the program
run: ${EXEC}
	./${EXEC}
# Clean up generated files
clean:
	rm -f ${OBJFILES} ${EXEC}

kb:
	setxkbmap pt
