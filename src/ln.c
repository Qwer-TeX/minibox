#include "config.h"
#ifdef CONFIG_LN
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

int ln(int argc, char *argv[]) {
  int sflag = 0;
  int i = 1;
  while (i < argc && argv[i][0] == '-') {
    if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--symbolic") == 0) sflag = 1;
    else { fprintf(stderr, "Usage: ln [-s] target linkname\n"); return 1; }
    i++;
  }
  if (argc - i != 2) {
    fprintf(stderr, "Usage: ln [-s] target linkname\n");
    return 1;
  }
  const char *target = argv[i];
  const char *linkname = argv[i+1];
  int ok = sflag ? symlink(target, linkname) : link(target, linkname);
  if (ok != 0) {
    perror("ln");
    return 1;
  }
  return 0;
}
#endif /* CONFIG_LN */
