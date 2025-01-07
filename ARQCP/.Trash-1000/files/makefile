# Variables
INCLUDES = func.h
SOURCES = func.c main.c 
OBJFILES = func.o main.o 
EXEC = prog

# Suffix rules 
.SUFFIXES : .c .o

# How to build an object .o from a code file .c ; $< -- file name 
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

# Debug 
debug: ${EXEC}
	 ddd ${EXEC} &