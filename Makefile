CC=gcc
CFLAGS= -g
LDFLAGS=
PROGS=wc cat cp sync yes update sleep whoami true false ls echo init rm rmdir\
			mv cmp

VERSION=v0.1.0

all: minibox strip


minibox: minibox.o links
	${CC} ${LDFLAGS} ${CFLAGS} -o minibox minibox.o
	ls -l $@
	size $@


minibox.o: minibox.c clean
	${CC} ${CFLAGS} -c minibox.c -o minibox.o

links:
	for f in ${PROGS}; do \
		ln -s minibox $$f > /dev/null 2>&1; done

strip: minibox
	$@ -vs $^

clean:
	rm -rf *.o minibox minibox-${VERSION} ${PROGS}


dist:
	tar czvf ../minibox-${VERSION}.tar.gz --exclude='.git' \
		--exclude='.gitignore' ../minibox
