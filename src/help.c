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

#include "minibox.h"

// Help function invoked from ./minibox as -h or --help
int help(int argc, char *argv[]) {
  printf("MiniBox %s: A multi-call binary that combines many common Unix "
         "utilities\n"
         "into one that aims to be lightweight and memory efficient.\n"
         "\n"
         "This is free software with ABSOLUTELY NO WARRANTY.\n"
         "For details see the LICENSE that came with this MiniBox "
         "distribution.\n"
         "\n"
         "Current implementations include (in chronological order from 1st to "
         "recently developed):\n"
#ifdef CONFIG_WC
         "wc:       Print newline, word, and byte counts\n"
#endif
#ifdef CONFIG_CAT
         "cat:      Concatenate files\n"
#endif
#ifdef CONFIG_CP
         "cp:       Copy files\n"
#endif
#ifdef CONFIG_SYNC
         "sync:     Sync filesystem caches to disk\n"
#endif
#ifdef CONFIG_YES
         "yes:      Output y or a character repeatedly until killed\n"
#endif
#ifdef CONFIG_UPDATE
         "update:   Sync filesystem caches every 30 seconds\n"
#endif
#ifdef CONFIG_SLEEP
         "sleep:    Sleep for the specified amount of seconds\n"
#endif
#ifdef CONFIG_WHOAMI
         "whoami:   Print current effective username\n"
#endif
#ifdef CONFIG_TRUE
         "true:     Return true\n"
#endif
#ifdef CONFIG_FALSE
         "false:    Return false\n"
#endif
#ifdef CONFIG_LS
         "ls:       List directory contents\n"
#endif
#ifdef CONFIG_ECHO
         "echo:     Display a line of text\n"
#endif
#ifdef CONFIG_INIT
         "init:     Initialize system\n"
#endif
#ifdef CONFIG_CMP
         "cmp:      Compare two files\n"
#endif
#ifdef CONFIG_RM
         "rm:       Remove files or directories\n"
#endif
#ifdef CONFIG_RMDIR
         "rmdir:    Remove empty directories\n"
#endif
#ifdef CONFIG_MKDIR
         "mkdir:    Create directories\n"
#endif
#ifdef CONFIG_MKNOD
         "mknod:    Create special files\n"
#endif
#ifdef CONFIG_HOSTNAME
         "hostname: Print hostname\n"
#endif
#ifdef CONFIG_FREE
         "free:     Display memory stats (shared & used probably broken)\n"
#endif
#ifdef CONFIG_XXD
         "xxd:      Make a hex dump\n"
#endif
#ifdef CONFIG_OD
         "od:       Dump files in octal format\n"
#endif
#ifdef CONFIG_HEXDUMP
         "hexdump:  Display a file in hexadecimal\n"
#endif
#ifdef CONFIG_W
         "w:        Display info about current users on machine (output "
         "broken)\n"
#endif
#ifdef CONFIG_VMSTAT
         "vmstat:   Report virtual memory statistics (output broken)\n"
#endif
#ifdef CONFIG_CUT
         "cut:      Exclude sections of lines in files and print to stdout\n"
#endif
#ifdef CONFIG_GREP
         "grep:     Print lines that match the pattern in files\n"
#endif
#ifdef CONFIG_TR
         "tr:       Translate characters\n"
#endif
#ifdef CONFIG_SORT
         "sort:     Sort lines of text\n"
#endif
#ifdef CONFIG_UNIQ
         "uniq:     Report or omit duplicate lines\n"
#endif
#ifdef CONFIG_UPTIME
         "uptime:   Display how long the system has been running\n"
#endif
#ifdef CONFIG_PS
         "ps:       Print current running processes snapshot\n"
#endif
#ifdef CONFIG_KILL
         "kill:     Send a signal to a process\n"
#endif
#ifdef CONFIG_TTY
         "tty:      Print the terminal connected to stdin\n"
#endif
#ifdef CONFIG_LINK
         "link:     Call link() to create a link to a file\n"
#endif
#ifdef CONFIG_UNLINK
         "unlink:   Call unlink() to remove a link to a file\n"
#endif
#ifdef CONFIG_NOHUP
         "nohup:    Run command immune to SIGHUP and all output to file\n"
#endif
#ifdef CONFIG_DIRNAME
         "dirname:  Strip last component from filename\n"
#endif
#ifdef CONFIG_BASENAME
         "basename: Strip directory and suffix from filenames\n"
#endif
#ifdef CONFIG_CAL
         "cal:      Display calendar\n"
#endif
#ifdef CONFIG_CLEAR
         "clear:    Clear the terminal screen\n"
#endif
#ifdef CONFIG_ENV
         "env:      Run a program in a custom environment\n"
#endif
#ifdef CONFIG_EXPAND
         "expand:   Convert tabs to spaces\n"
#endif
#ifdef CONFIG_UNEXPAND
         "unexpand: Convert spaces to tabs\n"
#endif
#ifdef CONFIG_FOLD
         "fold:     Wrap lines in input files to stdout\n"
#endif
#ifdef CONFIG_FACTOR
         "factor:   Display prime factors of a number\n"
#endif
#ifdef CONFIG_TOUCH
         "touch:    Create or change file timestamps\n"
#endif
#ifdef CONFIG_HEAD
         "head:     Output the first part of a file\n"
#endif
#ifdef CONFIG_TAIL
         "tail:     Output the last part of a file\n"
#endif
#ifdef CONFIG_PASTE
         "paste:    Merge lines from files\n"
#endif
#ifdef CONFIG_ARCH
         "arch:     Print machine hardware architecture\n"
#endif
#ifdef CONFIG_DATE
         "date:     Print/set system time and date\n"
#endif
         // Add more commands here as needed
         "\n",
         VERSION);
  return 0;
}
