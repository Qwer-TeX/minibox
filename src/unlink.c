#include "minibox.h"

/* unlink program */
int remove_link(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
    return 1;
  }

  if (unlink(argv[1]) != 0) {
    perror("unlink");
    return 1;
  }
  return 0;
}
