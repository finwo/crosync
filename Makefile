SRC=$(wildcard src/*.c)
CFLAGS?=-O3 -s
INCLUDES?=
LIBS?=

INCLUDES+=-I lib/rxi/dyad/src
SRC+=$(wildcard lib/rxi/dyad/src/*.c)

INCLUDES+=-I lib/antirez/rax
LIBS+=-lm
SRC+=lib/antirez/rax/crc16.c
SRC+=lib/antirez/rax/rax.c
SRC+=lib/antirez/rax/rc4rand.c

OBJ=$(SRC:.c=.o)

.c.o:
	$(CC) $(INCLUDES) $(LIBS) $(CFLAGS) -c -o $@ $^

crosync: $(OBJ)
	$(CC) $(INCLUDES) $(LIBS) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(OBJ)
