CC = gcc
CFLAGS = -lncurses -ltinfo

SRC = example2.c \
      readline/readline.c \
      readline/history.c \
      readline/funmap.c \
      readline/keymaps.c \
      readline/parens.c \
      readline/search.c \
      readline/shell.c \
      readline/terminal.c \
      readline/text.c \
      readline/util.c \
      readline/xmalloc.c

OBJ = $(SRC:.c=.o)

all: example2

example2: $(OBJ)
	$(CC) -rdynamic -o $@ $^ $(CFLAGS)
	strip -vs $@

clean:
	rm -f example2 $(OBJ)

