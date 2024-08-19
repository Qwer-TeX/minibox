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

/* init program */
/* Basic implementation of init - not production level yet */
int init(void) {
  printf("MiniBox %s init: Running init process\n", VERSION);
  printf("MiniBox %s init: World Initialized -- Hello, World!\n", VERSION);

  // Example: Starting essential services
  pid_t child_pid = fork();
  if (child_pid == 0) {
    // Child process
    execl("/bin/sh", "/bin/sh", "-c", "/sbin/networking", NULL);
    perror("execl");
    exit(1);
  } else if (child_pid > 0) {
    // Parent process
    printf("MiniBox %s init: Started networking service\n", VERSION);
  } else {
    // Error occurred
    perror("fork");
    exit(1);
  }

  while (1) {
    pause(); // Wait for signal
  }

  return 0;
}
