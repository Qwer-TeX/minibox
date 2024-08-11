#include "minibox.h"

/* sync program */
/* commit filesystem caches to disk - sync(2) */
int _sync(void) {
  sync();
  return 0;
}
