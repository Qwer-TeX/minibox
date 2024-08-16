#include "minibox.h"

int touch(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s file...\n", argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    const char *filename = argv[i];
    int fd =
        open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0) {
      perror("Error opening file");
      return 1;
    }
    close(fd);

    // Set the file's access and modification times to the current time
    struct utimbuf times;
    times.actime = times.modtime = time(NULL);
    if (utime(filename, &times) < 0) {
      perror("Error setting file times");
      return 1;
    }
  }

  return 0;
}
