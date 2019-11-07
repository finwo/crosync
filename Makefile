SRC=$(wildcard src/*.c)
CFLAGS?=-O3 -s
INCLUDES?=
LIBS?=

INCLUDES+=-I lib/rxi/dyad/src
SRC+=lib/rxi/dyad/src/dyad.c

OBJ=$(SRC:.c=.o)

.c.o:
	$(CC) $(INCLUDES) $(LIBS) $(CFLAGS) -c -o $@ $^

crosync: $(OBJ)
	$(CC) $(INCLUDES) $(LIBS) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(OBJ)
