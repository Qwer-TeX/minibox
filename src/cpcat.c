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

/* common code for the cp and cat program */
/* cat [infile] */
/* cp [source] [destination] */
int cpcat(const char *strmin, const char *strmout) {
  FILE *finptr;
  FILE *foutptr;
  int chars;

  if (strmin == NULL || strcmp(strmin, "-") == 0) {
    finptr = stdin;
  } else {
    finptr = fopen(strmin, "r");
    if (finptr == NULL) {
      fprintf(stderr, "Error: Cannot open input file %s\n", strmin);
      return 1;
    }
  }

  if (strmout == NULL || strcmp(strmout, "-") == 0) {
    foutptr = stdout;
  } else {
    foutptr = fopen(strmout, "w");
    if (foutptr == NULL) {
      fprintf(stderr, "Error: Cannot open output file %s\n", strmout);
      if (finptr != stdin)
        fclose(finptr);
      return 1;
    }
  }

  while ((chars = fgetc(finptr)) != EOF) {
    fputc(chars, foutptr);
  }

  if (finptr != stdin)
    fclose(finptr);
  if (foutptr != stdout)
    fclose(foutptr);

  return 0;
}
