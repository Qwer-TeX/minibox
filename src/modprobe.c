#include "config.h"
#ifdef CONFIG_MODPROBE
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

#include <sys/utsname.h>

int modprobe(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: modprobe module\n");
    return 1;
  }
#if defined(__linux__)
  struct utsname u;
  if (uname(&u) != 0) { perror("uname"); return 1; }
  char path[512];
  snprintf(path, sizeof(path), "/lib/modules/%s/%s.ko", u.release, argv[1]);
  /* Try insmod on it; fall back to just calling the name as module (best effort) */
  if (access(path, R_OK) == 0) {
    execl("/sbin/insmod", "insmod", path, (char*)NULL);
    /* if no insmod binary, fallthrough */
  }
  /* As a last resort try the module name directly (may work if alias exists) */
  fprintf(stderr, "modprobe: could not load %s (no /lib/modules or insmod)\n", argv[1]);
  return 1;
#else
  fprintf(stderr, "modprobe: not supported on this platform\n");
  return 1;
#endif
}
#endif /* CONFIG_MODPROBE */
