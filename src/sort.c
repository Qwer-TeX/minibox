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

// Comparison function for qsort
int compare_lines(const void *a, const void *b) {
  return strcmp(*(const char **)a, *(const char **)b);
}

/* sort program */
int sort(int argc, char *argv[]) {
  char *lines[1000]; // Array to store lines (simple fixed-size buffer)
  size_t count = 0;
  size_t capacity = sizeof(lines) / sizeof(lines[0]);

  // Read lines from standard input or file if provided
  FILE *file = stdin;
  if (argc == 2) {
    file = fopen(argv[1], "r");
    if (!file) {
      perror("Error opening file");
      return EXIT_FAILURE;
    }
  }

  char buffer[1024];
  while (fgets(buffer, sizeof(buffer), file)) {
    if (count >= capacity) {
      fprintf(stderr, "Too many lines to sort\n");
      return EXIT_FAILURE;
    }
    // Remove newline character and allocate memory for line
    buffer[strcspn(buffer, "\n")] = '\0';
    lines[count] = strdup(buffer);
    if (!lines[count]) {
      perror("Memory allocation failed");
      return EXIT_FAILURE;
    }
    count++;
  }

  if (file != stdin) {
    fclose(file);
  }

  // Sort the lines
  qsort(lines, count, sizeof(char *), compare_lines);

  // Print the sorted lines
  for (size_t i = 0; i < count; i++) {
    puts(lines[i]);
    free(lines[i]);
  }

  return EXIT_SUCCESS;
}
