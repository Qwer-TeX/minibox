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

/* mkdir program */
/* Create directories */
/* Usage: mkdir [-p] directory... */
int mkdir_cmd(int argc, char *argv[]) {
  int parents = 0;
  int start_index = 1;

  // Check for the -p flag
  if (argc > 1 && strcmp(argv[1], "-p") == 0) {
    parents = 1;
    start_index = 2;
  }

  if (argc < start_index) {
    fprintf(stderr, "Usage: mkdir [-p] directory...\n");
    return 1;
  }

  for (int i = start_index; i < argc; i++) {
    if (parents) {
      char *path = strdup(argv[i]);
      if (!path) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
      }

      char *p = path;
      while ((p = strchr(p, '/')) != NULL) {
        *p = '\0';
        if (mkdir(path, 0755) < 0 && errno != EEXIST) {
          fprintf(stderr, "mkdir: failed to create directory '%s': %s\n", path,
                  strerror(errno));
          free(path);
          return 1;
        }
        *p = '/';
        p++;
      }
      if (mkdir(argv[i], 0755) < 0 && errno != EEXIST) {
        fprintf(stderr, "mkdir: failed to create directory '%s': %s\n", argv[i],
                strerror(errno));
        free(path);
        return 1;
      }
      free(path);
    } else {
      if (mkdir(argv[i], 0755) < 0 && errno != EEXIST) {
        fprintf(stderr, "mkdir: failed to create directory '%s': %s\n", argv[i],
                strerror(errno));
        return 1;
      }
    }
  }

  return 0;
}
