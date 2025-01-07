ui: ui.o machmanager.o
	gcc -g ui.o machmanager.o -z noexecstack -o ui

ui.o: ui.c shared.h
	gcc -g -Wall -Wextra -fanalyzer -c ui.c -o ui.o

machmanager.o: machmanager.c shared.h
	gcc -g -Wall -Wextra -fanalyzer -c machmanager.c -o machmanager.o	

clean:
	rm -f *.o ui

run: ui
	./ui