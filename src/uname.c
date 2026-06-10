#include "config.h"
#ifdef CONFIG_UNAME
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

#include <sys/utsname.h>

int uname_cmd(int argc, char *argv[]) {
  struct utsname u;
  if (uname(&u) != 0) { perror("uname"); return 1; }
  int aflag = 0, sflag = 0, nflag = 0, rflag = 0, vflag = 0, mflag = 0, pflag = 0;
  if (argc == 1) { sflag = nflag = rflag = vflag = mflag = 1; }
  else {
    for (int i=1; i<argc; i++) {
      char *a = argv[i];
      if (a[0] != '-') { fprintf(stderr, "uname: unknown %s\n", a); return 1; }
      for (char *p = a+1; *p; p++) {
        if (*p=='a') aflag=1;
        else if (*p=='s') sflag=1;
        else if (*p=='n') nflag=1;
        else if (*p=='r') rflag=1;
        else if (*p=='v') vflag=1;
        else if (*p=='m') mflag=1;
        else if (*p=='p') pflag=1;
        else { fprintf(stderr, "uname: invalid option -- '%c'\n", *p); return 1; }
      }
    }
  }
  if (aflag) sflag=nflag=rflag=vflag=mflag=pflag=1;
  int first=1;
  if (sflag) { if(!first) printf(" "); printf("%s", u.sysname); first=0; }
  if (nflag) { if(!first) printf(" "); printf("%s", u.nodename); first=0; }
  if (rflag) { if(!first) printf(" "); printf("%s", u.release); first=0; }
  if (vflag) { if(!first) printf(" "); printf("%s", u.version); first=0; }
  if (mflag) { if(!first) printf(" "); printf("%s", u.machine); first=0; }
  if (pflag) { if(!first) printf(" "); printf("%s", u.machine); first=0; }
  printf("\n");
  return 0;
}
#endif /* CONFIG_UNAME */
