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

/* grep program */
int grep(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <pattern> <file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  const char *pattern = argv[1];
  const char *filename = argv[2];
  FILE *file = fopen(filename, "r");

  if (!file) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  char line[1024]; // Buffer to store each line read from the file
  while (fgets(line, sizeof(line), file)) {
    if (strstr(line, pattern)) { // Check if the pattern is in the current line
      printf("%s", line);        // Print the matching line
    }
  }

  fclose(file);
  return EXIT_SUCCESS;
}
