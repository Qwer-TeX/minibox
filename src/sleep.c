#include "minibox.h"

/* sleep program */
/* Usage: sleep [seconds] */
int _sleep(int argsc, char *argsv[]) {
  char *endptr;
  errno = 0;
  long secs;

  if (argsc != 2) {
    fprintf(stderr, "Usage: sleep [seconds]\n");
    return 1;
  }

  secs = strtol(argsv[1], &endptr, 10);

  if (errno != 0 || *endptr != '\0' || secs < 0) {
    fprintf(stderr, "Invalid number of seconds: %s\n", argsv[1]);
    return 1;
  }

  sleep((unsigned int)secs);
  return 0;
}
