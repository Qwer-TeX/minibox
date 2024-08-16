/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>.
 * All Rights Reserved.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
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

/* mknod program */
/* Usage: mknod name type [major minor] */
int mknod_command(int argc, char *argv[]) {
  if (argc < 4) {
    fprintf(stderr, "Usage: mknod name type [major minor]\n");
    return 1;
  }

  const char *name = argv[1];
  char type = argv[2][0];
  mode_t mode;
  dev_t dev = 0;

  switch (type) {
  case 'p':
    mode = S_IFIFO;
    break;
  case 'c':
  case 'u':
    if (argc < 6) {
      fprintf(stderr, "Usage: mknod name c [major minor]\n");
      return 1;
    }
    mode = S_IFCHR;
    dev = makedev(atoi(argv[3]), atoi(argv[4]));
    break;
  case 'b':
    if (argc < 6) {
      fprintf(stderr, "Usage: mknod name b [major minor]\n");
      return 1;
    }
    mode = S_IFBLK;
    dev = makedev(atoi(argv[3]), atoi(argv[4]));
    break;
  default:
    fprintf(stderr, "Invalid type: %c\n", type);
    return 1;
  }

  if (mknod(name, mode | 0666, dev) == -1) {
    perror("mknod");
    return 1;
  }

  return 0;
}
