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

/* free program */
/* Display the amount of free and used memory in the system */
/* FIXME: free displays 0 for shared while coreutils free displays non-zero */
int free_cmd(void) {
  FILE *file = fopen("/proc/meminfo", "r");
  if (!file) {
    perror("fopen");
    return 1;
  }

  char line[256];
  unsigned long total = 0, free = 0, available = 0, buffers = 0, cached = 0;

  while (fgets(line, sizeof(line), file)) {
    if (sscanf(line, "MemTotal: %lu kB", &total) == 1 ||
        sscanf(line, "MemFree: %lu kB", &free) == 1 ||
        sscanf(line, "MemAvailable: %lu kB", &available) == 1 ||
        sscanf(line, "Buffers: %lu kB", &buffers) == 1 ||
        sscanf(line, "Cached: %lu kB", &cached) == 1) {
      // Parsed a line
    }
  }

  fclose(file);

  unsigned long used = total - free - buffers - cached;

  printf("            total       used       free     shared  buff/cache  "
         "available\n");
  printf("Mem:   %10lu %10lu %10lu %10lu %10lu %10lu\n", total, used, free, 0UL,
         buffers + cached, available);

  return 0;
}
