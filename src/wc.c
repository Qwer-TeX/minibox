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

/* wc program */
/* Usage: wc [<] [infile] */
int wc(int argc, char *argv[]) {
  int print_lines = 1, print_words = 1, print_chars = 1;
  int total_lines = 0, total_words = 0, total_chars = 0;
  int i;
  FILE *file;

  // Parse command-line arguments
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "-h") == 0) {
        printf("Usage: wc [options] [file...]\n");
        printf("Options:\n");
        printf("  -l    Print the number of lines\n");
        printf("  -w    Print the number of words\n");
        printf("  -c    Print the number of characters\n");
        printf("  -m    Print the number of multibyte characters (not "
               "implemented)\n");
        printf("  -h    Display this help message\n");
        return 0;
      } else if (strcmp(argv[i], "-l") == 0) {
        print_words = 0;
        print_chars = 0;
      } else if (strcmp(argv[i], "-w") == 0) {
        print_lines = 0;
        print_chars = 0;
      } else if (strcmp(argv[i], "-c") == 0) {
        print_lines = 0;
        print_words = 0;
      } else if (strcmp(argv[i], "-m") == 0) {
        // Not implemented
      } else {
        fprintf(stderr, "Unknown option: %s\n", argv[i]);
        return 1;
      }
    }
  }

  if (argc == 1 || (argc == 2 && argv[1][0] == '-' && argv[1][1] == '\0')) {
    // No files specified, read from stdin
    int nline = 0, nword = 0, nchar = 0;
    int letter, inword = 0;

    while ((letter = getc(stdin)) != EOF) {
      ++nchar;
      if (letter == '\n')
        ++nline;
      if (letter == ' ' || letter == '\n' || letter == '\t')
        inword = 1;
      else if (inword == 0) {
        inword = 1;
        ++nword;
      }
    }
    if (print_lines)
      printf("%d ", nline);
    if (print_words)
      printf("%d ", nword);
    if (print_chars)
      printf("%d ", nchar);
    printf("\n");
  } else {
    // Files specified
    int total_file_lines = 0, total_file_words = 0, total_file_chars = 0;

    for (i = 1; i < argc; i++) {
      if (argv[i][0] == '-')
        continue; // Skip options

      file = fopen(argv[i], "r");
      if (file == NULL) {
        perror(argv[i]);
        continue;
      }

      int nline = 0, nword = 0, nchar = 0;
      int letter, inword = 0;
      while ((letter = getc(file)) != EOF) {
        ++nchar;
        if (letter == '\n')
          ++nline;
        if (letter == ' ' || letter == '\n' || letter == '\t')
          inword = 1;
        else if (inword == 0) {
          inword = 1;
          ++nword;
        }
      }
      fclose(file);

      if (print_lines || print_words || print_chars) {
        if (print_lines)
          printf("%d ", nline);
        if (print_words)
          printf("%d ", nword);
        if (print_chars)
          printf("%d ", nchar);
        printf("%s\n", argv[i]);
      }

      total_file_lines += nline;
      total_file_words += nword;
      total_file_chars += nchar;
    }

    // Print total counts if there were multiple files
    if (argc > 2) {
      if (print_lines)
        printf("%d ", total_file_lines);
      if (print_words)
        printf("%d ", total_file_words);
      if (print_chars)
        printf("%d ", total_file_chars);
      printf("total\n");
    }
  }

  return 0;
}
