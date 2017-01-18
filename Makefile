# Detect which compiler to use
CC = $(shell which musl-gcc tcc gcc 2>/dev/null | grep -v "not found" | head -n 1 || echo none)

# Basic configuration
CFLAGS = -I.
LDFLAGS =

default:
	$(CC) $(CFLAGS) main.c events.c messages.c -o tinycrud

clean:
	for f in `find | grep -e \\\.o$$` ; do echo $$f ; rm $$f ; done
	rm -f tinycrud
