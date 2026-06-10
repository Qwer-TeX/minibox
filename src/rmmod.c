#include "config.h"
#ifdef CONFIG_RMMOD
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

#include <sys/syscall.h>
#include <unistd.h>

int rmmod(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: rmmod module\n");
    return 1;
  }
#if defined(__linux__) && defined(SYS_delete_module)
  if (syscall(SYS_delete_module, argv[1], 0) != 0) {
    perror("rmmod");
    return 1;
  }
  return 0;
#else
  fprintf(stderr, "rmmod: not supported on this platform\n");
  return 1;
#endif
}
#endif /* CONFIG_RMMOD */
