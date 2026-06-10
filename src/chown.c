#include "config.h"
#ifdef CONFIG_CHOWN
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

/* chown program - minimal: user[:group] or uid[:gid] */
int chown_cmd(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: chown owner[:group] file...\n");
    return 1;
  }

  const char *owner = argv[1];
  char *group = strchr(owner, ':');
  uid_t uid = (uid_t)-1;
  gid_t gid = (gid_t)-1;

  if (group) {
    *group++ = '\0';
    if (*group) {
      struct group *gr = getgrnam(group);
      if (gr) gid = gr->gr_gid;
      else gid = (gid_t)atoi(group);
    }
  }

  struct passwd *pw = getpwnam(owner);
  if (pw) uid = pw->pw_uid;
  else if (*owner) uid = (uid_t)atoi(owner);

  for (int i = 2; i < argc; i++) {
    if (chown(argv[i], uid, gid) != 0) {
      perror("chown");
      return 1;
    }
  }
  return 0;
}
#endif /* CONFIG_CHOWN */
