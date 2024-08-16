/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>.
 * All Rights Reserved.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
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

/* echo program */
int echo(int argc, char *argv[]) {
  int n_flag = 0;
  int e_flag = 0;
  int E_flag = 1; // -E is default
  int arg_start = 1;

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
        for (int j = 1; argv[i][j] != '\0'; j++) {
          switch (argv[i][j]) {
          case 'n':
            n_flag = 1;
            break;
          case 'e':
            e_flag = 1;
            E_flag = 0;
            break;
          case 'E':
            E_flag = 1;
            e_flag = 0;
            break;
          default:
            fprintf(stderr, "Usage: echo [-neE] [string]\n");
            return 1;
          }
        }
        arg_start++;
      } else {
        break;
      }
    }
  }

  for (int i = arg_start; i < argc; i++) {
    if (e_flag && !E_flag) {
      for (char *p = argv[i]; *p != '\0'; p++) {
        if (*p == '\\') {
          switch (*(++p)) {
          case 'n':
            putchar('\n');
            break;
          case 't':
            putchar('\t');
            break;
          case '\\':
            putchar('\\');
            break;
          default:
            putchar('\\');
            putchar(*p);
            break;
          }
        } else {
          putchar(*p);
        }
      }
    } else {
      fputs(argv[i], stdout);
    }
    if (i < argc - 1) {
      putchar(' ');
    }
  }

  if (!n_flag) {
    putchar('\n');
  }

  return 0;
}
