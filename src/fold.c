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

#define DEFAULT_WIDTH 80

int fold(int argc, char *argv[]) {
  FILE *input = stdin;
  FILE *output = stdout;
  int width = DEFAULT_WIDTH;
  bool break_at_spaces = false;
  bool break_at_bytes = false;

  // Parse command-line arguments
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == 'w') {
      width = atoi(&argv[i][2]);
    } else if (argv[i][0] == '-' && argv[i][1] == 's') {
      break_at_spaces = true;
    } else if (argv[i][0] == '-' && argv[i][1] == 'b') {
      break_at_bytes = true;
    } else {
      input = fopen(argv[i], "r");
      if (!input) {
        perror("Error opening input file");
        return 1;
      }
    }
  }

  int c, column = 0, last_space = -1;

  while ((c = fgetc(input)) != EOF) {
    if (break_at_bytes) {
      if (column >= width) {
        fputc('\n', output);
        column = 0;
        last_space = -1;
      }
    } else {
      if (column >= width) {
        if (break_at_spaces && last_space != -1) {
          fseek(input, last_space, SEEK_SET);
          fputc('\n', output);
          column = 0;
          last_space = -1;
        } else {
          fputc('\n', output);
          column = 0;
        }
      }
    }

    if (c == ' ') {
      last_space = ftell(input);
    }

    fputc(c, output);
    column++;

    if (c == '\n') {
      column = 0;
      last_space = -1;
    }
  }

  if (column > 0 && !break_at_bytes && break_at_spaces && last_space != -1) {
    fseek(input, last_space, SEEK_SET);
    fputc('\n', output);
  }

  if (input != stdin) {
    fclose(input);
  }

  return 0;
}
