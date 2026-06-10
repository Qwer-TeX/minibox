#include "config.h"
#ifdef CONFIG_INSMOD
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

#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>

int insmod(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: insmod file [args...]\n");
    return 1;
  }
#if defined(__linux__)
  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) { perror(argv[1]); return 1; }
  const char *params = (argc > 2) ? argv[2] : "";
#if defined(SYS_finit_module)
  if (syscall(SYS_finit_module, fd, params, 0) != 0) {
    perror("insmod");
    close(fd);
    return 1;
  }
#else
  /* fallback would require reading whole file for init_module; keep simple */
  (void)params;
  fprintf(stderr, "insmod: finit_module not available\n");
  close(fd);
  return 1;
#endif
  close(fd);
  return 0;
#else
  fprintf(stderr, "insmod: not supported on this platform\n");
  return 1;
#endif
}
#endif /* CONFIG_INSMOD */
