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

/* vmstat program */
/* report virtual memory statistics */
/* FIXME: Outputs info not in their respective columns */
int vmstat(void) {
  struct sysinfo info;

  if (sysinfo(&info) != 0) {
    perror("sysinfo");
    return 1;
  }

  printf("procs -----------memory---------- ---swap-- -----io---- -system-- "
         "------cpu-----\n");
  printf(" r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us "
         "sy id wa st\n");
  printf(" %2d %2d %6ld %6ld %6ld %6ld %4ld %4ld %5ld %5ld %4d %4d %2d %2d %2d "
         "%2d %2d\n",
         0, 0, info.totalswap - info.freeswap, info.freeram / 1024,
         info.bufferram / 1024, (info.freeram - info.bufferram) / 1024, 0L, 0L,
         0L, 0L, 0, 0, 0, 0, 100, 0, 0);
  return 0;
}
