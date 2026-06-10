#include "config.h"
#ifdef CONFIG_CHMOD
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

static mode_t parse_mode(const char *m, mode_t cur) {
  mode_t who = 0, perm = 0;
  if (*m >= '0' && *m <= '7') {
    return (mode_t)strtol(m, NULL, 8);
  }
  while (*m == 'u' || *m == 'g' || *m == 'o' || *m == 'a') {
    if (*m == 'u') who |= 0700;
    else if (*m == 'g') who |= 0070;
    else if (*m == 'o') who |= 0007;
    else if (*m == 'a') who |= 0777;
    m++;
  }
  if (!who) who = 0777;
  char op = *m++;
  while (*m) {
    if (*m == 'r') perm |= 0444;
    else if (*m == 'w') perm |= 0222;
    else if (*m == 'x') perm |= 0111;
    m++;
  }
  if (op == '+') return cur | (perm & who);
  if (op == '-') return cur & ~(perm & who);
  if (op == '=') return (cur & ~who) | (perm & who);
  return cur;
}

int chmod_cmd(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: chmod mode file...\n");
    return 1;
  }
  const char *modestr = argv[1];
  int ret = 0;
  for (int i=2; i<argc; i++) {
    struct stat st;
    if (stat(argv[i], &st) != 0) { perror(argv[i]); ret=1; continue; }
    mode_t nm = parse_mode(modestr, st.st_mode & 0777);
    if (chmod(argv[i], nm) != 0) { perror(argv[i]); ret=1; }
  }
  return ret;
}
#endif /* CONFIG_CHMOD */
