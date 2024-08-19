/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>.
 * All Rights Reserved.
 *
 * Licensed under WTFPL, see file LICENSE in this source tree.
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
 * Licensed under WTFPL, see file LICENSE in this source tree.
 */

#include "minibox.h"

/* uptime program */
int uptime() {
  struct sysinfo info;

  if (sysinfo(&info) != 0) {
    perror("sysinfo");
    return EXIT_FAILURE;
  }

  // Calculate uptime in days, hours, minutes, and seconds
  long uptime_seconds = info.uptime;
  long days = uptime_seconds / (24 * 3600);
  uptime_seconds %= (24 * 3600);
  long hours = uptime_seconds / 3600;
  uptime_seconds %= 3600;
  long minutes = uptime_seconds / 60;
  long seconds = uptime_seconds % 60;

  // Print uptime
  printf("uptime: %ld day(s), %ld:%02ld:%02ld\n", days, hours, minutes,
         seconds);

  return EXIT_SUCCESS;
}
