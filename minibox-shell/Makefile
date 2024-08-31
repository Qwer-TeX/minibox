CC=gcc
CFLAGS=
LDFLAGS=

all: sash examples

sash: minibox-shell.o
	$(CC) -s -o $@ $^ linenoise/linenoise.c
	ln -s sash sh

minibox-shell.o: minibox-shell.c
	$(CC) -s -Os -Ilinenoise -c $^

examples: example1 example2 example3

example1:
	$(CC) -s -o example1 example.c

example2:
	$(CC) -s -o example2 example2.c -l:libreadline.a -ltinfo -lc -Wl,-Bdynamic

example3:
	$(CC) -s -Os -Ilinenoise -o example3 example3.c linenoise/linenoise.c

clean:
	rm -rf *.o sash sh example1 example2 example3

