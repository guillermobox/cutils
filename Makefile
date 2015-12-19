CFLAGS += -Wall -pedantic -O2
Q := @

all: darray.o

darray.o: darray.c

%.o: %.c
	@echo "  CC  " $@
	$(Q)$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean all

clean:
	rm -f *.o
