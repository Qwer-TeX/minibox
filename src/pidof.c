#include "config.h"
#ifdef CONFIG_PIDOF
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

#include <dirent.h>

int pidof(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: pidof program\n");
    return 1;
  }
  const char *name = argv[1];
  DIR *d = opendir("/proc");
  if (!d) { perror("/proc"); return 1; }
  struct dirent *e;
  int found = 0;
  while ((e = readdir(d))) {
    if (e->d_type != DT_DIR) continue;
    char *end; long pid = strtol(e->d_name, &end, 10);
    if (*end || pid <= 0) continue;
    char path[256]; snprintf(path, sizeof(path), "/proc/%ld/comm", pid);
    FILE *f = fopen(path, "r");
    if (!f) continue;
    char comm[256]; if (fgets(comm, sizeof(comm), f)) {
      comm[strcspn(comm, "\n")] = 0;
      if (strcmp(comm, name) == 0) {
        if (found) printf(" ");
        printf("%ld", pid);
        found = 1;
      }
    }
    fclose(f);
  }
  closedir(d);
  if (found) printf("\n");
  return found ? 0 : 1;
}
#endif /* CONFIG_PIDOF */
