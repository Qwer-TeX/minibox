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

/* tty program */
int tty(int argc, char *argv[]) {
  char *tty_path;

  // Get the path of the terminal device
  tty_path = ttyname(STDIN_FILENO);

  if ((argc == 2) && (!strcmp(argv[1], "-s"))) {
    // Do nothing since invoked as silent
  } else {
    // Print tty path or a warning if not tty
    puts((tty_path != NULL) ? tty_path : "not a tty");
  }

  return 0;
}
