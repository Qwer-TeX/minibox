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
