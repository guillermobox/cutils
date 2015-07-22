CFLAGS += -g -Wall

all: main_head main_open main_linked

main_head: hashset_head.o main.o
	$(CC) $^ -o $@

main_open: hashset_open.o main.o
	$(CC) $^ -o $@

main_linked: hashset_linked.o main.o
	$(CC) $^ -o $@

.PHONY: clean all

clean:
	rm -f *.o main_*
