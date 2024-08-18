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

static void print_usage(const char *prog_name) {
  fprintf(
      stderr,
      "Usage: %s [-c num] [-n num] [file]\n"
      "  -c num  Output the first `num` characters.\n"
      "  -n num  Output the first `num` lines.\n"
      "  file     File to read; if not provided, reads from standard input.\n",
      prog_name);
}

int head(int argc, char *argv[]) {
  int num_lines = 10; // Default number of lines
  int num_chars = -1; // Default: no limit on number of characters

  // Parse command-line arguments manually
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "-c") == 0) {
        if (i + 1 < argc) {
          num_chars = atoi(argv[++i]);
          if (num_chars < 0) {
            fprintf(stderr, "Invalid number of characters: %d\n", num_chars);
            return EXIT_FAILURE;
          }
        } else {
          fprintf(stderr, "Option -c requires an argument.\n");
          print_usage(argv[0]);
          return EXIT_FAILURE;
        }
      } else if (strcmp(argv[i], "-n") == 0) {
        if (i + 1 < argc) {
          num_lines = atoi(argv[++i]);
          if (num_lines < 0) {
            fprintf(stderr, "Invalid number of lines: %d\n", num_lines);
            return EXIT_FAILURE;
          }
        } else {
          fprintf(stderr, "Option -n requires an argument.\n");
          print_usage(argv[0]);
          return EXIT_FAILURE;
        }
      } else {
        print_usage(argv[0]);
        return EXIT_FAILURE;
      }
    } else {
      // File argument found
      FILE *input = fopen(argv[i], "r");
      if (input == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
      }

      // Read and output the desired content
      if (num_chars >= 0) {
        // Output the first `num_chars` characters
        int c;
        int count = 0;
        while ((c = fgetc(input)) != EOF &&
               (num_chars < 0 || count < num_chars)) {
          putchar(c);
          count++;
        }
      } else {
        // Output the first `num_lines` lines
        char buffer[BUFSIZ];
        int count = 0;
        while (fgets(buffer, sizeof(buffer), input) != NULL &&
               (num_lines < 0 || count < num_lines)) {
          fputs(buffer, stdout);
          count++;
        }
      }

      fclose(input);
      return EXIT_SUCCESS;
    }
  }

  // If no file argument, use stdin
  FILE *input = stdin;
  if (num_chars >= 0) {
    // Output the first `num_chars` characters
    int c;
    int count = 0;
    while ((c = fgetc(input)) != EOF && (num_chars < 0 || count < num_chars)) {
      putchar(c);
      count++;
    }
  } else {
    // Output the first `num_lines` lines
    char buffer[BUFSIZ];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), input) != NULL &&
           (num_lines < 0 || count < num_lines)) {
      fputs(buffer, stdout);
      count++;
    }
  }

  return EXIT_SUCCESS;
}
