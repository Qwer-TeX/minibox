/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>,
 * Enhanced by Stephen M. Jones <support@actmac.com>
 *            -- All Rights Reserved.
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

/* cmp program */
/* Usage: cmp [comparer] [comparent] */
int cmp(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: cmp <file1> <file2>\n");
    return 2;
  }

  const char *file1 = argv[1];
  const char *file2 = argv[2];

  FILE *f1 = fopen(file1, "rb");
  FILE *f2 = fopen(file2, "rb");
  if (!f1 || !f2) {
    if (f1)
      fclose(f1);
    if (f2)
      fclose(f2);
    fprintf(stderr, "cmp: Cannot open file\n");
    return 2;
  }

  int ret = 0;
  while (1) {
    int ch1 = fgetc(f1);
    int ch2 = fgetc(f2);
    if (ch1 != ch2) {
      ret = 1;
      break;
    }
    if (ch1 == EOF)
      break;
  }

  if (ret)
    printf("Files differ\n");
  fclose(f1);
  fclose(f2);
  return ret;
}
