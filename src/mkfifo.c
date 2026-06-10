#include "config.h"
#ifdef CONFIG_MKFIFO
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

#include <sys/stat.h>

int mkfifo_cmd(int argc, char *argv[]) {
  mode_t mode = 0666;
  int i = 1;
  if (i < argc && argv[i][0] == '-' && argv[i][1] == 'm') {
    // very minimal: ignore detailed mode for tiny size; accept -m but use default
    i++;
    if (i < argc) i++;
  }
  if (i >= argc) {
    fprintf(stderr, "Usage: mkfifo [-m mode] name...\n");
    return 1;
  }
  int ret = 0;
  for (; i < argc; i++) {
    if (mkfifo(argv[i], mode) != 0) {
      perror(argv[i]);
      ret = 1;
    }
  }
  return ret;
}
#endif /* CONFIG_MKFIFO */
