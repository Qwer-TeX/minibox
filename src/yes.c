#include "minibox.h"

/* yes program */
/* output y or a character repeatedly until killed */
/* yes [repeatent] */
int yes(char *args[]) {
  while (1) {
    printf("%s\n", args ? args[1] : "y");
  }
  return 0;
}
