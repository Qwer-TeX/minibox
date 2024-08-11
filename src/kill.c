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

// Function to convert signal name to signal number
int signal_name_to_number(const char *name) {
  if (name == NULL)
    return -1;

  // List of known signals
  static const struct {
    const char *name;
    int number;
  } signals[] = {
      {"SIGHUP", SIGHUP},       {"SIGINT", SIGINT},       {"SIGQUIT", SIGQUIT},
      {"SIGILL", SIGILL},       {"SIGTRAP", SIGTRAP},     {"SIGABRT", SIGABRT},
      {"SIGBUS", SIGBUS},       {"SIGFPE", SIGFPE},       {"SIGKILL", SIGKILL},
      {"SIGUSR1", SIGUSR1},     {"SIGSEGV", SIGSEGV},     {"SIGUSR2", SIGUSR2},
      {"SIGPIPE", SIGPIPE},     {"SIGALRM", SIGALRM},     {"SIGTERM", SIGTERM},
      {"SIGSTKFLT", SIGSTKFLT}, {"SIGCHLD", SIGCHLD},     {"SIGCONT", SIGCONT},
      {"SIGSTOP", SIGSTOP},     {"SIGTSTP", SIGTSTP},     {"SIGTTIN", SIGTTIN},
      {"SIGTTOU", SIGTTOU},     {"SIGURG", SIGURG},       {"SIGXCPU", SIGXCPU},
      {"SIGXFSZ", SIGXFSZ},     {"SIGVTALRM", SIGVTALRM}, {"SIGPROF", SIGPROF},
      {"SIGWINCH", SIGWINCH},   {"SIGIO", SIGIO},         {"SIGPWR", SIGPWR},
      {"SIGSYS", SIGSYS}};

  for (size_t i = 0; i < sizeof(signals) / sizeof(signals[0]); i++) {
    if (strcmp(name, signals[i].name) == 0) {
      return signals[i].number;
    }
  }
  return -1; // Signal name not found
}

/* kill program */
int kill_process(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s [-signal] PID...\n", argv[0]);
    return EXIT_FAILURE;
  }

  int signal = SIGTERM; // Default signal

  if (argc >= 3 && argv[1][0] == '-') {
    if (argv[1][1] == 'S') {
      // Named signal (e.g., -SIGHUP)
      signal = signal_name_to_number(argv[1] + 1);
      if (signal == -1) {
        fprintf(stderr, "Unknown signal: %s\n", argv[1]);
        return EXIT_FAILURE;
      }
    } else {
      // Signal number (e.g., -9)
      char *endptr;
      signal = strtol(argv[1] + 1, &endptr, 10);
      if (*endptr != '\0' || signal <= 0 || signal >= NSIG) {
        fprintf(stderr, "Invalid signal number: %s\n", argv[1]);
        return EXIT_FAILURE;
      }
    }
    // Shift arguments
    argv++;
    argc--;
  }

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [-signal] PID...\n", argv[0]);
    return EXIT_FAILURE;
  }

  for (int i = 1; i < argc; i++) {
    char *endptr;
    pid_t pid = strtol(argv[i], &endptr, 10);
    if (*endptr != '\0' || pid <= 0) {
      fprintf(stderr, "Invalid PID: %s\n", argv[i]);
      return EXIT_FAILURE;
    }

    if (kill(pid, signal) == -1) {
      if (errno == ESRCH) {
        fprintf(stderr, "No such process: %d\n", pid);
      } else if (errno == EPERM) {
        fprintf(stderr, "Permission denied to send signal to PID %d\n", pid);
      } else {
        perror("kill");
      }
      return EXIT_FAILURE;
    }

    printf("Sent signal %d to PID %d\n", signal, pid);
  }

  return EXIT_SUCCESS;
}
