#include "config.h"
#ifdef CONFIG_LSMOD
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

int lsmod(int argc, char *argv[]) {
  FILE *f = fopen("/proc/modules", "r");
  if (!f) {
    fprintf(stderr, "lsmod: /proc/modules not available\n");
    return 1;
  }
  char buf[256];
  printf("Module                  Size  Used by\n");
  while (fgets(buf, sizeof(buf), f)) {
    char name[64]; unsigned long size; int usecount;
    if (sscanf(buf, "%63s %lu %d", name, &size, &usecount) >= 2) {
      printf("%-19s %8lu  %d\n", name, size, usecount);
    }
  }
  fclose(f);
  return 0;
}
#endif /* CONFIG_LSMOD */
