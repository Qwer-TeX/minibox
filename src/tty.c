/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>.
 * All Rights Reserved.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 * minibox-shell.c is the main shell source with all shell builtins.
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
int tty(void) {
  char *tty_path;
  int fd;

  // Get the path of the terminal device
  tty_path = ttyname(STDIN_FILENO);
  if (!tty_path) {
    fprintf(stderr, "Not a terminal.\n");
    return 1;
  }

  // Attempt to open the terminal device to ensure it's valid
  fd = open(tty_path, O_RDONLY);
  if (fd < 0) {
    perror("tty");
    return 1;
  }

  // Print the terminal path
  printf("%s\n", tty_path);

  // Close the file descriptor
  close(fd);

  return 0;
}
