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

/* cmp program */
/* Usage: cmp [comparer] [comparent] */
int cmp(const char *file1, const char *file2) {
  FILE *f1 = fopen(file1, "r");
  FILE *f2 = fopen(file2, "r");
  int ch1, ch2;

  if (!f1 || !f2) {
    fprintf(stderr, "cmp: Cannot open file\n");
    return 2;
  }

  while ((ch1 = fgetc(f1)) != EOF && (ch2 = fgetc(f2)) != EOF) {
    if (ch1 != ch2) {
      printf("Files differ\n");
      fclose(f1);
      fclose(f2);
      return 1;
    }
  }

  if ((ch1 != EOF) || (ch2 != EOF)) {
    printf("Files differ\n");
    fclose(f1);
    fclose(f2);
    return 1;
  }

  fclose(f1);
  fclose(f2);
  return 0;
}
