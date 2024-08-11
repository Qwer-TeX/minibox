#include "minibox.h"

/* rmdir program */
/* Remove empty directories */
/* Usage: rmdir directory... */
int rmdir_cmd(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: rmdir directory...\n");
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    if (rmdir(argv[i]) < 0) {
      fprintf(stderr, "rmdir: failed to remove '%s': %s\n", argv[i],
              strerror(errno));
      return 1;
    }
  }

  return 0;
}
