CFLAGS += -Wall -pedantic -O2
Q := @

all: darray.o btree.o

darray.o: darray.c

btree.o: btree.c

%.o: %.c
	@echo "  CC  " $@
	$(Q)$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean all

clean:
	rm -f *.o
