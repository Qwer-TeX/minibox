#include "config.h"
#ifdef CONFIG_DF
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

#include <sys/statvfs.h>

int df(int argc, char *argv[]) {
  int human = 0;
  int i=1;
  if (i < argc && strcmp(argv[i], "-h") == 0) { human=1; i++; }
  if (i >= argc) {
    /* very mini: just show a header and note */
    printf("Filesystem     Size  Used Avail Use%% Mounted on\n");
    printf("df: listing /proc/mounts or /etc/mtab not fully implemented\n");
    return 0;
  }
  for (; i < argc; i++) {
    struct statvfs v;
    if (statvfs(argv[i], &v) != 0) {
      perror(argv[i]);
      continue;
    }
    unsigned long bsize = v.f_frsize ? v.f_frsize : v.f_bsize;
    unsigned long total = (unsigned long)v.f_blocks * bsize;
    unsigned long avail = (unsigned long)v.f_bavail * bsize;
    unsigned long used = total - avail;
    if (human) {
      printf("%s: %lu bytes total, %lu used, %lu avail\n", argv[i], total, used, avail);
    } else {
      printf("%s: %lu %lu %lu\n", argv[i], total/1024, used/1024, avail/1024);
    }
  }
  return 0;
}
#endif /* CONFIG_DF */
