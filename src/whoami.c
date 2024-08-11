#include "minibox.h"

/* whoami program */
/* print current effective user */
/* Usage: whoami */
int whoami(void) {
  struct passwd *pw_ent = getpwuid(geteuid());
  if (pw_ent == NULL) {
    fprintf(stderr, "Error: Cannot determine current user\n");
    return 1;
  }
  printf("%s\n", pw_ent->pw_name);
  return 0;
}
