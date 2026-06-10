#include "config.h"
#ifdef CONFIG_POWEROFF
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

#if defined(__linux__)
#include <sys/reboot.h>
#endif

int poweroff(int argc, char *argv[]) {
  const char *base = basename(argv[0]);
#if defined(__linux__) && defined(RB_POWER_OFF)
  int cmd = RB_POWER_OFF;
  if (strstr(base, "reboot")) cmd = RB_AUTOBOOT;
  else if (strstr(base, "halt")) cmd = RB_HALT_SYSTEM;
  if (reboot(cmd) != 0) {
    perror(base);
    return 1;
  }
  return 0;
#else
  fprintf(stderr, "%s: not supported on this platform\n", base);
  return 1;
#endif
}
#endif /* CONFIG_POWEROFF */
