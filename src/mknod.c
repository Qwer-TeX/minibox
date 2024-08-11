#include "minibox.h"

/* mknod program */
/* Usage: mknod name type [major minor] */
int mknod_command(int argc, char *argv[]) {
  if (argc < 4) {
    fprintf(stderr, "Usage: mknod name type [major minor]\n");
    return 1;
  }

  const char *name = argv[1];
  char type = argv[2][0];
  mode_t mode;
  dev_t dev = 0;

  switch (type) {
  case 'p':
    mode = S_IFIFO;
    break;
  case 'c':
  case 'u':
    if (argc < 6) {
      fprintf(stderr, "Usage: mknod name c [major minor]\n");
      return 1;
    }
    mode = S_IFCHR;
    dev = makedev(atoi(argv[3]), atoi(argv[4]));
    break;
  case 'b':
    if (argc < 6) {
      fprintf(stderr, "Usage: mknod name b [major minor]\n");
      return 1;
    }
    mode = S_IFBLK;
    dev = makedev(atoi(argv[3]), atoi(argv[4]));
    break;
  default:
    fprintf(stderr, "Invalid type: %c\n", type);
    return 1;
  }

  if (mknod(name, mode | 0666, dev) == -1) {
    perror("mknod");
    return 1;
  }

  return 0;
}
