#include "minibox.h"

/* basename program */
int print_basename(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <path>\n", argv[0]);
    return 1;
  }

  char *path_copy = strdup(argv[1]);
  if (!path_copy) {
    perror("strdup");
    return 1;
  }

  char *base = basename(path_copy);
  printf("%s\n", base);

  free(path_copy);
  return 0;
}
