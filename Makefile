CC = gcc
CFLAGS = -Oz -flto -Wall -Wextra -Wno-unused-result -Iinclude
LDFLAGS = -flto
EXEC = minibox_unstripped

PROGS = wc cp cat sync yes update sleep whoami true false ls echo init cmp rm \
				rmdir mkdir mknod hostname free xxd od hexdump w vmstat cut grep tr sort uniq \
				uptime ps kill tty link unlink nohup dirname basename cal clear env expand \
				unexpand fold factor touch head tail paste

SRCS = $(addprefix src/, $(PROGS:=.c)) src/minibox.c
OBJS = $(SRCS:.c=.o)

VERSION=v0.3.1

all: $(EXEC) strip

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

strip: $(EXEC)
	# Lets reduce the executable size even more by removing useless sections
	$@ -s -R .note -R .comment -R .symtab -R.strtab $^ -o minibox
	ls -l minibox
	size minibox

tags:
	ls src/*.c | xargs ctags -a

dist:
	tar czvf ../minibox-${VERSION}.tar.gz --exclude='.git' \
		--exclude='.gitignore' ../minibox

links:
	@for i in ${PROGS}; do \
		ln -s minibox $$i > /dev/null 2>&1; done

clean:
	rm -rf minibox $(EXEC) $(OBJS) tags install_dir

.PHONY: all strip tags dist links install clean

