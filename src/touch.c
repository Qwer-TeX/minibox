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

int touch(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s file...\n", argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    const char *filename = argv[i];
    int fd =
        open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0) {
      perror("Error opening file");
      return 1;
    }
    close(fd);

    // Set the file's access and modification times to the current time
    struct utimbuf times;
    times.actime = times.modtime = time(NULL);
    if (utime(filename, &times) < 0) {
      perror("Error setting file times");
      return 1;
    }
  }

  return 0;
}
