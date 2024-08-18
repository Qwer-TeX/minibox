/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 AUTHOR <AUTHOR@MAIL_SERVER.protocol>.
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
#include "utils.h"

// Put its definition in utils.h or if other utilites would use this function,
// define it in utils.h
void do_this() {
  printf("This is a template used to code in more utilites for MiniBox\n");
}

/* program example */
// Its defintion is in minibox.h
int skel(int argc, char *argv[]) {
  puts("My function");
  if (argc < 2) {
    fprintf(stderr, "Usage: %s [-lsdfv]\n", argv[0]);
    return 1;
  }
  do_this();
  return 0;
}
