#include "minibox.h"

/* update program */
int update(void) {
  for (int i = 0; i != 3; ++i)
    close(i);
  chdir("/");
  while (1) {
    _sync();
    sleep(30);
  }
}
