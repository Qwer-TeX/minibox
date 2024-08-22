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

/* rm program */
/* Remove files or directories */
/* Usage: rm [-r] file... */
int rm(int argc, char *argv[]) {
  int recursive = 0;

  // Check for the -r flag
  if (argc > 1 && strcmp(argv[1], "-r") == 0) {
    recursive = 1;
    argc--;
    argv++;
  }

  if (argc < 2) {
    fprintf(stderr, "Usage: rm [-r] file...\n");
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    struct stat st;
    if (stat(argv[i], &st) < 0) {
      fprintf(stderr, "rm: cannot stat '%s': %s\n", argv[i], strerror(errno));
      return 1;
    }

    if (S_ISDIR(st.st_mode)) {
      if (recursive) {
        // Recursively delete directory contents
        DIR *dir = opendir(argv[i]);
        if (dir == NULL) {
          fprintf(stderr, "rm: cannot open directory '%s': %s\n", argv[i],
                  strerror(errno));
          return 1;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
          if (strcmp(entry->d_name, ".") != 0 &&
              strcmp(entry->d_name, "..") != 0) {
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "%s/%s", argv[i], entry->d_name);
            if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
              // Recursively remove subdirectories
              char *args[] = {"rm", "-r", path};
              rm(3, args);
            } else {
              unlink(path);
            }
          }
        }
        closedir(dir);
        rmdir(argv[i]);
      } else {
        fprintf(stderr, "rm: '%s' is a directory\n", argv[i]);
        return 1;
      }
    } else {
      if (unlink(argv[i]) < 0) {
        fprintf(stderr, "rm: cannot remove '%s': %s\n", argv[i],
                strerror(errno));
        return 1;
      }
    }
  }

  return 0;
}
