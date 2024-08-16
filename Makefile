CC = gcc
CFLAGS = -Oz -flto -Wall -Wextra -Wno-unused-result -Iinclude
LDFLAGS = -flto
EXEC = minibox_unstripped

PROGS = wc cp cat sync yes update sleep whoami true false ls echo init cmp rm \
				rmdir mkdir mknod hostname free xxd od hexdump w vmstat cut grep tr sort uniq \
				uptime ps kill tty link unlink nohup dirname basename cal clear env expand \
				unexpand fold factor touch head tail paste

SRCS = src/minibox.c src/wc.c src/cpcat.c src/sync.c src/yes.c src/update.c src/sleep.c \
			 src/whoami.c src/true.c src/false.c src/ls.c src/echo.c src/init.c src/cmp.c \
			 src/rm.c src/rmdir.c src/mkdir.c src/mknod.c src/hostname.c src/free.c src/xxd.c \
       src/od.c src/hexdump.c src/w.c src/vmstat.c src/cut.c src/grep.c src/tr.c \
			 src/sort.c src/uniq.c src/uptime.c src/ps.c src/kill.c src/tty.c src/link.c \
			 src/unlink.c src/nohup.c src/dirname.c src/basename.c src/cal.c src/clear.c \
			 src/env.c src/expand.c src/unexpand.c src/fold.c src/factor.c src/touch.c \
			 src/head.c src/tail.c src/paste.c
OBJS = $(SRCS:.c=.o)

VERSION=v0.3.1

all: $(EXEC) strip

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

strip: $(EXEC)
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

