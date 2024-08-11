#include "minibox.h"

/* dirname program */
int print_dirname(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <path>\n", argv[0]);
    return 1;
  }

  char *path_copy = strdup(argv[1]);
  if (!path_copy) {
    perror("strdup");
    return 1;
  }

  char *dir = dirname(path_copy);
  printf("%s\n", dir);

  free(path_copy);
  return 0;
}
