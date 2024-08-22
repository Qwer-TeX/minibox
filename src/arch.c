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

static void print_usage() {
  printf("Usage: arch [-h]\n");
  printf("  -h      Display this help message\n");
}

int arch(int argc, char *argv[]) {
  if (argc > 2) {
    print_usage();
    return 1;
  }

  if (argc == 2) {
    if (argv[1][0] == '-' && argv[1][1] == 'h' && argv[1][2] == '\0') {
      print_usage();
      return 0;
    } else {
      print_usage();
      return 1;
    }
  }

  // Default behavior: print machine architecture
  struct utsname uname_data;
  if (uname(&uname_data) == 0) {
    printf("%s\n", uname_data.machine);
  } else {
    perror("uname");
    return 1;
  }

  return 0;
}
