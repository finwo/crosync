SRC=$(wildcard src/*.c)
CFLAGS?=-O3 -s
INCLUDES?=

INCLUDES+=-I lib/rxi/dyad/src
SRC+=lib/rxi/dyad/src/dyad.c

OBJ=$(SRC:.c=.o)

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $^

crosync: $(OBJ)
	$(CC) $(INCLUDES) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(OBJ)
