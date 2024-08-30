/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>.
 * All Rights Reserved.
 *
 * Licensed under Unlicense License, see file LICENSE in this source tree.
 *
 * When adding programs or features, please consider if they can be
 * accomplished in a sane way with standard unix tools. If they're
 * programs or features you added, please make sure they are read-
 * able and understandable by a novice-advanced programmer, if not,
 * add comments or let me know. Use common sense and please don't
 * bloat sources.
 *
 * I haven't tested but it could compile on windows systems with MSYS/MinGW or
 * Cygwin. MiniBox should be fairly portable for POSIX systems.
 *
 * Licensed under Unlicense License, see file LICENSE in this source tree.
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#ifdef _WIN32
#include <direct.h>
#include <fcntl.h>
#include <io.h>
#include <process.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <windows.h>
#define PATH_MAX MAX_PATH
#else
/* local compatibility headers under include to increase UNIX coverage */
#include "limits.h"
#include "signals.h"
#include "stat.h"
#include "sysmacros.h"
#include "utmp.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <libgen.h>
#include <pwd.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/reboot.h>
/* For the BSDs, libsysinfo port is required and `-I/usr/local/include` must be added to CFLAGS in Makefile */
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <utime.h>
#endif


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// MiniBox specific defines
#ifndef VERSION
#define VERSION "undef_ver"
#endif

// Function to compare two directory entries for qsort
int compare_entries(const void *a, const void *b);

// Comparison function for qsort
int compare_lines(const void *a, const void *b);

// Function to convert signal name to signal number
int signal_name_to_number(const char *name);

/* wc program */
int wc(int argc, char *argv[]);

/* cat [infile] */
int cat(int argc, char *argv[]);
/* cp [source] [destination] */
int cp(int argc, char *argv[]);

/* sync program */
int _sync(int argc, char *argv[]);

/* update program */
int update(int argc, char *argv[]);

/* yes program */
int yes(int argc, char *argv[]);

/* sleep program */
int _sleep(int argc, char *argv[]);

/* whoami program */
int whoami(int argc, char *argv[]);

/* true program */
int _true(int argc, char *argv[]);

/* false program */
int _false(int argc, char *argv[]);

/* ls program */
int ls(int argc, char *argv[]);

/* echo program */
int echo(int argc, char *argv[]);

/* init program */
int init(int argc, char *argv[]);

/* cmp program */
int cmp(int argc, char *argv[]);

/* rm program */
int rm(int argc, char *argv[]);

/* rmdir program */
int rmdir_cmd(int argc, char *argv[]);

/* mkdir program */
int mkdir_cmd(int argc, char *argv[]);

/* mknod program */
int mknod_command(int argc, char *argv[]);

/* hostname program */
int hostname(int argc, char *argv[]);

/* free program */
int free_cmd(int argc, char *argv[]);

/* xxd program */
int xxd(int argc, char *argv[]);

/* od program */
int od(int argc, char *argv[]);

/* hexdump program */
int hexdump(int argc, char *argv[]);

/* w program */
int w(int argc, char *argv[]);

/* vmstat program */
int vmstat(int argc, char *argv[]);

/* cut program */
int cut(int argc, char *argv[]);

/* grep program */
int grep(int argc, char *argv[]);

/* tr program */
int tr(int argc, char *argv[]);

/* sort program */
int sort(int argc, char *argv[]);

/* uniq program */
int uniq(int argc, char *argv[]);

/* uptime program */
int uptime(int argc, char *argv[]);

/* ps program */
int ps(int argc, char *argv[]);

/* kill program */
int kill_process(int argc, char *argv[]);

/* tty program */
int tty(int argc, char *argv[]);

/* link program */
int create_link(int argc, char *argv[]);

/* unlink program */
int remove_link(int argc, char *argv[]);

/* nohup program */
int nohup(int argc, char *argv[]);

/* dirname program */
int print_dirname(int argc, char *argv[]);

/* basename program */
int print_basename(int argc, char *argv[]);

/* cal program */
int cal(int argc, char *argv[]);

/* clear program */
int clear(int argc, char *argv[]);

/* env program */
int env(int argc, char *argv[]);

/* expand program */
int expand(int argc, char *argv[]);

/* unexpand program */
int unexpand(int argc, char *argv[]);

/* fold program */
int fold(int argc, char *argv[]);

/* factor program */
int factor(int argc, char *argv[]);

/* touch program */
int touch(int argc, char *argv[]);

/* head program */
int head(int argc, char *argv[]);

/* tail program */
int tail(int argc, char *argv[]);

/* paste program */
int paste(int argc, char *argv[]);

/* arch program */
int arch(int argc, char *argv[]);

/* date program */
int date(int argc, char *argv[]);

/* help function */ // Not a program
int help(int argc, char *argv[]);

/* version function */ // Not a program
int version(int argc, char *argv[]);

#endif // !COMMANDS_H
