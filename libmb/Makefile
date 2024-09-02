CC			= gcc
CFLAGS	= -g -Oz -Wall -Wextra -I../include
FUNC		= xzalloc xmalloc xrealloc xfopen
SOURCES	= $(FUNC:=.c)
OBJECTS = $(SOURCES:.c=.o)
LIB			= libmb/libmb.a

all: $(OBJECTS)
	ar rcs libmb.a $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf libmb.a $(OBJECTS)

distclean: clean

