/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>.
 * All Rights Reserved.
 *
 * Licensed under WTFPL, see file LICENSE in this source tree.
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
 * Licensed under WTFPL, see file LICENSE in this source tree.
 */

#include "minibox.h"
#include "utils.h"

// Function to check if invoked as minibox (inline so no function overhead)
int check_if_minibox(char *argv0) {
  return strcmp(basename(argv0), "minibox") == 0;
}

// Command-function pointer type
typedef int (*CommandFunc)(int argc, char *argv[]);

// Command struct
typedef struct {
  const char *cmd_name;
  CommandFunc cmd_func;
} Command;

// Create command table
Command commands[] = {
    {"wc", (CommandFunc)wc},
    {"cat", cat},
    {"cp", cp},
    {"sync", _sync},
    {"yes", yes},
    {"update", update},
    {"sleep", _sleep},
    {"whoami", whoami},
    {"true", _true},
    {"false", _false},
    {"ls", ls},
    {"echo", echo},
    {"init", init},
    {"cmp", (CommandFunc)cmp},
    {"rm", rm},
    {"rmdir", rmdir_cmd},
    {"mkdir", mkdir_cmd},
    {"mknod", mknod_command},
    {"hostname", hostname},
    {"free", free_cmd},
    {"xxd", xxd},
    {"od", od},
    {"hexdump", hexdump},
    {"w", w},
    {"vmstat", vmstat},
    {"cut", cut},
    {"grep", grep},
    {"tr", tr},
    {"sort", sort},
    {"uniq", uniq},
    {"uptime", uptime},
    {"ps", ps},
    {"kill", kill_process},
    {"tty", tty},
    {"link", create_link},
    {"unlink", remove_link},
    {"nohup", nohup},
    {"dirname", print_dirname},
    {"basename", print_basename},
    {"cal", cal},
    {"clear", clear},
    {"env", env},
    {"expand", expand},
    {"unexpand", unexpand},
    {"fold", fold},
    {"factor", factor},
    {"touch", touch},
    {"head", head},
    {"tail", tail},
    {"paste", paste},
    {"arch", arch},
    {"date", date}
    // Add more commands here
};

size_t num_commands = sizeof(commands) / sizeof(commands[0]);

// Function to execute a command based on the input
int execute_command(const char *cmd, int argc, char *argv[]) {
  for (size_t i = 0; i < num_commands; ++i) {
    if (strcmp(cmd, commands[i].cmd_name) == 0) {
      return commands[i].cmd_func(argc, argv);
    }
  }
  fprintf(stderr, "Unknown command: %s\n", cmd);
  return 1;
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
  } else if (strcmp(cmd, "cmp") == 0) {
    if (argc != 3) {
      fprintf(stderr, "Usage: cmp [file1] [file2]\n");
      return 1;
    }
    return cmp(argv[1], argv[2]);
  } else if (!strcmp(cmd, "-h") || !strcmp(cmd, "--help")) {
    return help(argc, argv);
  }

  return execute_command(cmd, argc, argv);
  // else {
  //   printf("MiniBox %s: A multi-call binary that combines many common Unix "
  //          "utilities\n"
  //          "into one that aims to be lightweight and memory efficient.\n"
  //          "\n"
  //          "This is free software with ABSOLUTELY NO WARRANTY.\n"
  //          "For details see the LICENSE that came with this MiniBox "
  //          "distribution.\n"
  //          "\n"
  //          "Current implementations include (in chronological order from 1st
  //          " "to recently developed):\n" "wc:       Print newline, word, and
  //          byte counts\n" "cat:      Concatenate files\n" "cp:       Copy
  //          files\n" "sync:     Sync filesystem caches to disk\n" "yes: Output
  //          y or a character repeatedly until killed\n" "update:   Sync
  //          filesystem caches every 30 seconds\n" "sleep:    Sleep for the
  //          specified amount of seconds\n" "whoami:   Print current effective
  //          username\n" "true:     Return true\n" "false:    Return false\n"
  //          "ls:       List directory contents\n"
  //          "echo:     Display a line of text\n"
  //          "init:     Initialize system\n"
  //          "cmp:      Compare two files\n"
  //          "rm:       Remove files or directories\n"
  //          "rmdir:    Remove empty directories\n"
  //          "mkdir:    Create directories\n"
  //          "mknod:    Create special files\n"
  //          "hostname: Print hostname\n"
  //          "free:     Display memory stats (shared & used probably broken)\n"
  //          "xxd:      Make a hex dump\n"
  //          "od:       Dump files in octal format\n"
  //          "hexdump:  Display a file in hexadecimal\n"
  //          "w:        Display info about current users on machine (output "
  //          "broken)\n"
  //          "vmstat:   Report virtual memory statistics (output broken)\n"
  //          "cut:      Exclude sections of lines in files and print to
  //          stdout\n" "grep:     Print lines that match the pattern in
  //          files\n" "tr:       Translate characters\n" "sort:     Sort lines
  //          of text\n" "uniq:     Report or omit duplicate lines\n" "uptime:
  //          Display how long the system has been running\n" "ps:       Print
  //          current running processes snapshot\n" "kill:     Send a signal to
  //          a process\n" "tty:      Print the terminal connected to stdin\n"
  //          "link:     Call link() to create a link to a file\n"
  //          "unlink:   Call unlink() to remove a link to a file\n"
  //          "nohup:    Run command immune to SIGHUP and all output to file\n"
  //          "dirname:  Strip last component from filename\n"
  //          "basename: Strip directory and suffix from filenames\n"
  //          "cal:      Display calendar\n"
  //          "clear:    Clear the terminal screen\n"
  //          "env:      Run a program in a custom environment\n"
  //          "expand:   Convert tabs to spaces\n"
  //          "unexpand: Convert spaces to tabs\n"
  //          "fold:     Wrap lines in input files to stdout\n"
  //          "factor:   Display prime factors of a number\n"
  //          "touch:    Create or change file timestamps\n"
  //          "head:     Output the first part of a file\n"
  //          "tail:     Output the last part of a file\n"
  //          "paste:    Merge lines from files\n"
  //          "arch:     Print machine hardware architecture\n"
  //          "date:     print/set system time and date\n",
  //          VERSION);
  //   return 1;
  // }
}
