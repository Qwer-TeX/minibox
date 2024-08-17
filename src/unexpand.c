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

#define DEFAULT_TAB_WIDTH 8

int unexpand(int argc, char *argv[]) {
  FILE *input = stdin;
  FILE *output = stdout;
  int tab_width = DEFAULT_TAB_WIDTH;
  bool convert_all_spaces = false;
  bool skip_form_feed = false;

  // Parse command-line arguments
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == 't') {
      tab_width = atoi(&argv[i][2]);
    } else if (argv[i][0] == '-' && argv[i][1] == 'a') {
      convert_all_spaces = true;
    } else if (argv[i][0] == '-' && argv[i][1] == 'f') {
      skip_form_feed = true;
    } else {
      input = fopen(argv[i], "r");
      if (!input) {
        perror("Error opening input file");
        return 1;
      }
    }
  }

  int c;
  int space_count = 0;

  while ((c = fgetc(input)) != EOF) {
    if (c == ' ') {
      space_count++;
    } else {
      if (space_count > 0) {
        if (skip_form_feed && (c == '\f')) {
          fputc(c, output);
        } else {
          if (convert_all_spaces) {
            int tabs = space_count / tab_width;
            int spaces = space_count % tab_width;
            for (int i = 0; i < tabs; i++)
              fputc('\t', output);
            for (int i = 0; i < spaces; i++)
              fputc(' ', output);
          } else {
            for (int i = 0; i < space_count; i++)
              fputc(' ', output);
          }
        }
        space_count = 0;
      }
      fputc(c, output);
    }
  }

  // Handle remaining spaces
  if (space_count > 0) {
    if (skip_form_feed) {
      fputc(' ', output);
    } else {
      if (convert_all_spaces) {
        int tabs = space_count / tab_width;
        int spaces = space_count % tab_width;
        for (int i = 0; i < tabs; i++)
          fputc('\t', output);
        for (int i = 0; i < spaces; i++)
          fputc(' ', output);
      } else {
        for (int i = 0; i < space_count; i++)
          fputc(' ', output);
      }
    }
  }

  if (input != stdin)
    fclose(input);

  return 0;
}
