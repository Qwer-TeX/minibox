#include "config.h"
#ifdef CONFIG_MOUNT
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

#if defined(__linux__)
#include <sys/mount.h>
#endif

/* mount program - very minimal */
int mount_cmd(int argc, char *argv[]) {
  const char *type = NULL;
  const char *options = NULL;
  int i = 1;

  while (i < argc && argv[i][0] == '-') {
    if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
      type = argv[++i];
    } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
      options = argv[++i];
    } else if (strcmp(argv[i], "-r") == 0) {
      options = options ? "ro" : "ro";
    } else {
      fprintf(stderr, "Usage: mount [-t type] [-o options] [-r] source target\n");
      return 1;
    }
    i++;
  }
  (void)type; /* used only on __linux__ in mount(2) call */

  if (argc - i < 2) {
    FILE *f = fopen("/proc/mounts", "r");
    if (!f) f = fopen("/etc/mtab", "r");
    if (!f) { fprintf(stderr, "mount: no mounts\n"); return 1; }
    char buf[512];
    while (fgets(buf, sizeof(buf), f)) fputs(buf, stdout);
    fclose(f);
    return 0;
  }

  if (argc - i != 2) {
    fprintf(stderr, "Usage: mount [-t type] [-o options] [-r] source target\n");
    return 1;
  }

#if defined(__linux__)
  const char *src = argv[i];
  const char *tgt = argv[i + 1];
  unsigned long flags = 0;
  if (options && strstr(options, "ro")) flags |= MS_RDONLY;

  if (mount(src, tgt, type ? type : "none", flags, options && *options ? options : NULL) != 0) {
    perror("mount");
    return 1;
  }
  return 0;
#else
  fprintf(stderr, "mount: not supported on this platform\n");
  return 1;
#endif
}
#endif /* CONFIG_MOUNT */
