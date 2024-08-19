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

int xflag = 0;
int hflag = 0;

int clear(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      for (int j = 1; argv[i][j]; j++) {
        switch (argv[i][j]) {
        case 'x':
          xflag++;
          break;
        case 'h':
          hflag++;
          break;
        default:
          fprintf(stderr, "Invalid option\n");
          fprintf(stderr, "Usage: clear [-xh]\n");
          exit(1);
        }
      }
    }
  }
  if (xflag) {
    printf("\033[H\033[2J");
  } else if (hflag) {
    printf("Usage: clear [-xh]\n");
  } else {
    printf("\033[H\033[2J\033[3J");
  }

  return 0;
}
