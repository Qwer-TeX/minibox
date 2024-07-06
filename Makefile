CC=gcc
CFLAGS=
LDFLAGS=

VERSION=v0.1.0pre3-unstable

all: minibox links


minibox: minibox.o
	${CC} -o minibox minibox.o
	strip -vs $@


minibox.o: minibox.c
	${CC} -c minibox.c -o minibox.o

links: minibox
	ln -s minibox wc
	ln -s minibox cat
	ln -s minibox cp
	ln -s minibox sync
	ln -s minibox yes
	ln -s minibox update


clean:
	rm -rf *.o minibox minibox-dist wc cat cp sync yes update


dist:
	tar czvf ../minibox-${VERSION}.tar.gz --exclude='.git' \
		--exclude='.gitignore' ../minibox
