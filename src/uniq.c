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

/* uniq program */
int uniq(int argc, char *argv[]) {
  char *prev_line = NULL;
  char buffer[1024];

  // Read lines from standard input or file if provided
  FILE *file = stdin;
  if (argc == 2) {
    file = fopen(argv[1], "r");
    if (!file) {
      perror("Error opening file");
      return EXIT_FAILURE;
    }
  }

  while (fgets(buffer, sizeof(buffer), file)) {
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

    if (prev_line == NULL || strcmp(prev_line, buffer) != 0) {
      if (prev_line != NULL) {
        puts(prev_line);
        free(prev_line);
      }
      prev_line = strdup(buffer);
      if (!prev_line) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
      }
    }
  }

  if (prev_line != NULL) {
    puts(prev_line);
    free(prev_line);
  }

  if (file != stdin) {
    fclose(file);
  }

  return EXIT_SUCCESS;
}
