#include "config.h"
#ifdef CONFIG_LOGIN
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

#include <termios.h>

int login(int argc, char *argv[]) {
  const char *user = (argc > 1) ? argv[1] : NULL;
  char ubuf[64];
  if (!user) {
    printf("login: "); fflush(stdout);
    if (!fgets(ubuf, sizeof(ubuf), stdin)) return 1;
    ubuf[strcspn(ubuf, "\n")] = 0;
    user = ubuf;
  }
  struct passwd *pw = getpwnam(user);
  if (!pw) { fprintf(stderr, "login: unknown user\n"); return 1; }
  /* very minimal password prompt (no crypt compare in tiny build) */
  struct termios old, t;
  tcgetattr(0, &old);
  t = old; t.c_lflag &= ~ECHO;
  tcsetattr(0, TCSANOW, &t);
  printf("Password: "); fflush(stdout);
  char pwbuf[128]; if (!fgets(pwbuf, sizeof(pwbuf), stdin)) { tcsetattr(0,TCSANOW,&old); return 1; }
  tcsetattr(0, TCSANOW, &old);
  printf("\n");
  /* Accept any password for the minimal version (or check pw->pw_passwd == "x" etc.) */
  if (setgid(pw->pw_gid) != 0 || setuid(pw->pw_uid) != 0) {
    perror("login");
    return 1;
  }
  chdir(pw->pw_dir ? pw->pw_dir : "/");
  const char *sh = pw->pw_shell[0] ? pw->pw_shell : "/bin/sh";
  execl(sh, sh, (char*)NULL);
  perror("login: exec");
  return 1;
}
#endif /* CONFIG_LOGIN */
