#include "minibox.h"

/* hostname program */
int hostname(void) {
  char hostname[1024];
  if (gethostname(hostname, sizeof(hostname)) == -1) {
    perror("gethostname");
    return 1;
  }
  printf("%s\n", hostname);
  return 0;
}
