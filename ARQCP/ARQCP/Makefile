CC = gcc
CFLAGS = -g -Wall -Wextra -fanalyzer
LDFLAGS =

SRCS = main.c machmanager.c ui.c
ASM_SRCS = extract_data.s get_number_binary.s enqueue_value.s dequeue_value.s get_n_element.s move_n_to_array.s sort_array.s median.s
OBJS = $(SRCS:.c=.o) $(ASM_SRCS:.s=.o)

all: prog

prog: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	as -g -o $@ $<

clean:
	rm -f $(OBJS) prog

run: prog
	./prog