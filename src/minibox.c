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

// Create command table conditionally
Command commands[] = {
#ifdef CONFIG_WC
    {"wc", wc},
#endif
#ifdef CONFIG_CAT
    {"cat", cat},
#endif
#ifdef CONFIG_CP
    {"cp", cp},
#endif
#ifdef CONFIG_SYNC
    {"sync", _sync},
#endif
#ifdef CONFIG_YES
    {"yes", yes},
#endif
#ifdef CONFIG_UPDATE
    {"update", update},
#endif
#ifdef CONFIG_SLEEP
    {"sleep", _sleep},
#endif
#ifdef CONFIG_WHOAMI
    {"whoami", whoami},
#endif
#ifdef CONFIG_TRUE
    {"true", _true},
#endif
#ifdef CONFIG_FALSE
    {"false", _false},
#endif
#ifdef CONFIG_LS
    {"ls", ls},
#endif
#ifdef CONFIG_ECHO
    {"echo", echo},
#endif
#ifdef CONFIG_INIT
    {"init", init},
#endif
#ifdef CONFIG_CMP
    {"cmp", cmp},
#endif
#ifdef CONFIG_RM
    {"rm", rm},
#endif
#ifdef CONFIG_RMDIR
    {"rmdir", rmdir_cmd},
#endif
#ifdef CONFIG_MKDIR
    {"mkdir", mkdir_cmd},
#endif
#ifdef CONFIG_MKNOD
    {"mknod", mknod_command},
#endif
#ifdef CONFIG_HOSTNAME
    {"hostname", hostname},
#endif
#ifdef CONFIG_FREE
    {"free", free_cmd},
#endif
#ifdef CONFIG_XXD
    {"xxd", xxd},
#endif
#ifdef CONFIG_OD
    {"od", od},
#endif
#ifdef CONFIG_HEXDUMP
    {"hexdump", hexdump},
#endif
#ifdef CONFIG_W
    {"w", w},
#endif
#ifdef CONFIG_VMSTAT
    {"vmstat", vmstat},
#endif
#ifdef CONFIG_CUT
    {"cut", cut},
#endif
#ifdef CONFIG_GREP
    {"grep", grep},
#endif
#ifdef CONFIG_TR
    {"tr", tr},
#endif
#ifdef CONFIG_SORT
    {"sort", sort},
#endif
#ifdef CONFIG_UNIQ
    {"uniq", uniq},
#endif
#ifdef CONFIG_UPTIME
    {"uptime", uptime},
#endif
#ifdef CONFIG_PS
    {"ps", ps},
#endif
#ifdef CONFIG_KILL
    {"kill", kill_process},
#endif
#ifdef CONFIG_TTY
    {"tty", tty},
#endif
#ifdef CONFIG_LINK
    {"link", create_link},
#endif
#ifdef CONFIG_UNLINK
    {"unlink", remove_link},
#endif
#ifdef CONFIG_NOHUP
    {"nohup", nohup},
#endif
#ifdef CONFIG_DIRNAME
    {"dirname", print_dirname},
#endif
#ifdef CONFIG_BASENAME
    {"basename", print_basename},
#endif
#ifdef CONFIG_CAL
    {"cal", cal},
#endif
#ifdef CONFIG_CLEAR
    {"clear", clear},
#endif
#ifdef CONFIG_ENV
    {"env", env},
#endif
#ifdef CONFIG_EXPAND
    {"expand", expand},
#endif
#ifdef CONFIG_UNEXPAND
    {"unexpand", unexpand},
#endif
#ifdef CONFIG_FOLD
    {"fold", fold},
#endif
#ifdef CONFIG_FACTOR
    {"factor", factor},
#endif
#ifdef CONFIG_TOUCH
    {"touch", touch},
#endif
#ifdef CONFIG_HEAD
    {"head", head},
#endif
#ifdef CONFIG_TAIL
    {"tail", tail},
#endif
#ifdef CONFIG_PASTE
    {"paste", paste},
#endif
#ifdef CONFIG_ARCH
    {"arch", arch},
#endif
#ifdef CONFIG_DATE
    {"date", date},
#endif
    // Add more commands here as needed
};

size_t num_commands = sizeof(commands) / sizeof(commands[0]);

// Function to execute a command based on the input
int execute_command(const char *cmd, int argc, char *argv[]) {
  for (size_t i = 0; i < num_commands; ++i) {
    if (strcmp(cmd, commands[i].cmd_name) == 0) {
      return commands[i].cmd_func(argc, argv);
    }
  }
  fprintf(stderr, "Unknown command or option specified: %s\n", cmd);
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

  if (!strcmp(cmd, "cmp")) {
    if (argc != 3) {
      fprintf(stderr, "Usage: cmp [file1] [file2]\n");
      return 1;
    }
    return cmp(argv[1], argv[2]);
  } else if (!strcmp(cmd, "-h") || !strcmp(cmd, "--help")) {
    return help(argc, argv);
  } else if (!strcmp(cmd, "-v") || !strcmp(cmd, "--version")) {
    printf("MiniBox %s\n", VERSION);
    return 0;
  }

  return execute_command(cmd, argc, argv);
}
