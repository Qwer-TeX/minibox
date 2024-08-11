/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>.
 * All Rights Reserved.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 * minibox-shell.c is the main shell source with all shell builtins.
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
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <linux/limits.h>
#include <linux/stat.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <utmp.h>

/* minibox specific defines */
#define VERSION "0.2.2"

// Function to compare two directory entries for qsort
int compare_entries(const void *a, const void *b);

// Comparison function for qsort
int compare_lines(const void *a, const void *b);

// Function to convert signal name to signal number
int signal_name_to_number(const char *name);

/* wc program */
int wc(FILE *strmin, FILE *strmout);

/* cat [infile] */
/* cp [source] [destination] */
int cpcat(const char *strmin, const char *strmout);

/* sync program */
int _sync(void);

/* update program */
int update(void);

/* yes program */
int yes(char *args[]);

/* sleep program */
int _sleep(int argsc, char *argsv[]);

/* whoami program */
int whoami(void);

/* true program */
int _true(void);

/* false program */
int _false(void);

/* ls program */
int ls(int argc, char *argv[]);

/* echo program */
int echo(int argc, char *argv[]);

/* init program */
int init(void);

/* cmp program */
int cmp(const char *file1, const char *file2);

/* rm program */
int rm(int argc, char *argv[]);

/* rmdir program */
int rmdir_cmd(int argc, char *argv[]);

/* mkdir program */
int mkdir_cmd(int argc, char *argv[]);

/* mknod program */
int mknod_command(int argc, char *argv[]);

/* hostname program */
int hostname(void);

/* free program */
int free_cmd(void);

/* xxd program */
int xxd(int argc, char *argv[]);

/* od program */
int od(int argc, char *argv[]);

/* hexdump program */
int hexdump(int argc, char *argv[]);

/* w program */
int w(void);

/* vmstat program */
int vmstat(void);

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
int uptime(void);

/* ps program */
int ps(void);

/* kill program */
int kill_process(int argc, char *argv[]);

/* tty program */
int tty(void);

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

#endif // !COMMANDS_H
