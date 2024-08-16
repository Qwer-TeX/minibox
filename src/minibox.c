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

#include "minibox.h"
#include "utils.h"
#include <string.h>

// Function to check if invoked as minibox (inline so no function overhead)
int check_if_minibox(char *argv0) {
  return strcmp(basename(argv0), "minibox") == 0;
}

/* Update main function */
int main(int argc, char *argv[]) {
  int is_minibox = check_if_minibox(argv[0]);

  if (is_minibox && argc > 1) {
    argc--; // Decrease the count to exclude the program name
    argv++; // Move argv to exclude the program name
  }
  if (argc < 1) {
    fprintf(stderr, "No command specified\n");
    return 1;
  }

  char *cmd = basename(argv[0]);

  if (strcmp(cmd, "wc") == 0) {
    if (argc == 1) {
      return wc(stdin, stdout);
    } else {
      // FIXME: Integrate this into the wc function
      FILE *f = fopen(argv[1], "r");
      if (!f) {
        fprintf(stderr, "wc: %s: No such file or directory\n", argv[1]);
        return 1;
      }
      int res = wc(f, stdout);
      fclose(f);
      return res;
    }
  } else if (strcmp(cmd, "cat") == 0) {
    return cpcat(argc > 1 ? argv[1] : NULL, NULL);
  } else if (strcmp(cmd, "cp") == 0) {
    if (argc != 3) {
      fprintf(stderr, "Usage: cp [source] [destination]\n");
      return 1;
    }
    return cpcat(argv[1], argv[2]);
  } else if (strcmp(cmd, "sync") == 0) {
    return _sync();
  } else if (strcmp(cmd, "yes") == 0) {
    return yes(argc > 1 ? argv : NULL);
  } else if (strcmp(cmd, "update") == 0) {
    return update();
  } else if (strcmp(cmd, "sleep") == 0) {
    return _sleep(argc, argv);
  } else if (strcmp(cmd, "whoami") == 0) {
    return whoami();
  } else if (strcmp(cmd, "true") == 0) {
    return _true();
  } else if (strcmp(cmd, "false") == 0) {
    return _false();
  } else if (strcmp(cmd, "ls") == 0) {
    return ls(argc, argv);
  } else if (strcmp(cmd, "echo") == 0) {
    return echo(argc, argv);
  } else if (strcmp(cmd, "init") == 0) {
    return init();
  } else if (strcmp(cmd, "cmp") == 0) {
    if (argc != 3) {
      fprintf(stderr, "Usage: cmp [file1] [file2]\n");
      return 1;
    }
    return cmp(argv[1], argv[2]);
  } else if (strcmp(cmd, "rm") == 0) {
    return rm(argc, argv);
  } else if (strcmp(cmd, "rmdir") == 0) {
    return rmdir_cmd(argc, argv);
  } else if (strcmp(cmd, "mkdir") == 0) {
    return mkdir_cmd(argc, argv);
  } else if (strcmp(cmd, "mknod") == 0) {
    return mknod_command(argc, argv);
  } else if (strcmp(cmd, "hostname") == 0) {
    return hostname();
  } else if (strcmp(cmd, "free") == 0) {
    return free_cmd();
  } else if (strcmp(cmd, "xxd") == 0) {
    return xxd(argc, argv);
  } else if (strcmp(cmd, "od") == 0) {
    return od(argc, argv);
  } else if (strcmp(cmd, "hexdump") == 0) {
    return hexdump(argc, argv);
  } else if (strcmp(cmd, "w") == 0) {
    return w();
  } else if (strcmp(cmd, "vmstat") == 0) {
    return vmstat();
  } else if (strcmp(cmd, "cut") == 0) {
    return cut(argc, argv);
  } else if (strcmp(cmd, "grep") == 0) {
    return grep(argc, argv);
  } else if (strcmp(cmd, "tr") == 0) {
    return tr(argc, argv);
  } else if (strcmp(cmd, "sort") == 0) {
    return sort(argc, argv);
  } else if (strcmp(cmd, "uniq") == 0) {
    return uniq(argc, argv);
  } else if (strcmp(cmd, "uptime") == 0) {
    return uptime();
  } else if (strcmp(cmd, "ps") == 0) {
    return ps();
  } else if (strcmp(cmd, "kill") == 0) {
    return kill_process(argc, argv);
  } else if (strcmp(cmd, "tty") == 0) {
    return tty(argc, argv);
  } else if (strcmp(cmd, "link") == 0) {
    return create_link(argc, argv);
  } else if (strcmp(cmd, "unlink") == 0) {
    return remove_link(argc, argv);
  } else if (strcmp(cmd, "nohup") == 0) {
    return nohup(argc, argv);
  } else if (strcmp(cmd, "dirname") == 0) {
    return print_dirname(argc, argv);
  } else if (strcmp(cmd, "basename") == 0) {
    return print_basename(argc, argv);
  } else if (strcmp(cmd, "cal") == 0) {
    return cal(argc, argv);
  } else {
    printf("MiniBox %s: A multi-call binary that combines many common Unix "
           "utilities\n"
           "into one that aims to be lightweight and memory efficient.\n"
           "\n"
           "This is free software with ABSOLUTELY NO WARRANTY.\n"
           "For details see the LICENSE that came with this MiniBox "
           "distribution.\n"
           "\n"
           "Current implementations include (in chronological order from 1st "
           "to recently developed):\n"
           "wc:       Print newline, word, and byte counts\n"
           "cat:      Concatenate files\n"
           "cp:       Copy files\n"
           "sync:     Sync filesystem caches to disk\n"
           "yes:      Output y or a character repeatedly until killed\n"
           "update:   Sync filesystem caches every 30 seconds\n"
           "sleep:    Sleep for the specified amount of seconds\n"
           "whoami:   Print current effective username\n"
           "true:     Return true\n"
           "false:    Return false\n"
           "ls:       List directory contents\n"
           "echo:     Display a line of text\n"
           "init:     Initialize system\n"
           "cmp:      Compare two files\n"
           "rm:       Remove files or directories\n"
           "rmdir:    Remove empty directories\n"
           "mkdir:    Create directories\n"
           "mknod:    Create special files\n"
           "hostname: Print hostname\n"
           "free:     Display memory stats (shared & used probably broken)\n"
           "xxd:      Make a hex dump\n"
           "od:       Dump files in octal format\n"
           "hexdump:  Display a file in hexadecimal\n"
           "w:        Display info about current users on machine (output "
           "broken)\n"
           "vmstat:   Report virtual memory statistics (output broken)\n"
           "cut:      Exclude sections of lines in files and print to stdout\n"
           "grep:     Print lines that match the pattern in files\n"
           "tr:       Translate characters\n"
           "sort:     Sort lines of text\n"
           "uniq:     Report or omit duplicate lines\n"
           "uptime:   Display how long the system has been running\n"
           "ps:       Print current running processes snapshot\n"
           "kill:     Send a signal to a process\n"
           "tty:      Print the terminal connected to stdin\n"
           "link:     Call link() to create a link to a file\n"
           "unlink:   Call unlink() to remove a link to a file\n"
           "nohup:    Run command immune to SIGHUP and all output to file\n"
           "dirname:  Strip last component from filename\n"
           "basename: Strip directory and suffix from filenames\n"
           "cal:      Display calendar\n",
           VERSION);
    return 1;
  }
}
