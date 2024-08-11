#include "minibox.h"

/* link program */
int create_link(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s FILE1 FILE2\n", argv[0]);
    return 1;
  }

  if (link(argv[1], argv[2]) != 0) {
    perror("link");
    return 1;
  }
  return 0;
}
