#include "config.h"
#ifdef CONFIG_SU
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

int su(int argc, char *argv[]) {
  const char *user = "root";
  if (argc > 1) user = argv[1];
  struct passwd *pw = getpwnam(user);
  if (!pw) { fprintf(stderr, "su: unknown user %s\n", user); return 1; }
  if (setgid(pw->pw_gid) != 0 || setuid(pw->pw_uid) != 0) {
    perror("su: setuid");
    return 1;
  }
  if (chdir(pw->pw_dir) != 0) { /* best effort */ }
  const char *sh = pw->pw_shell[0] ? pw->pw_shell : "/bin/sh";
  execl(sh, sh, (char*)NULL);
  perror("su: exec shell");
  return 1;
}
#endif /* CONFIG_SU */
