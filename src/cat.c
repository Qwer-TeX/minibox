#include "minibox.h"

#define BUFFER_SIZE 1024

int cat(int argc, char *argv[]) {
  char buffer[BUFFER_SIZE];
  ssize_t bytes_read;
  int fd;

  if (argc == 1) {
    // No files provided, read from stdin
    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0) {
      if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read) {
        perror("Error writing to stdout");
        return EXIT_FAILURE;
      }
    }

    if (bytes_read < 0) {
      perror("Error reading from stdin");
      return EXIT_FAILURE;
    }
  } else {
    // Files provided, read from each file
    for (int i = 1; i < argc; i++) {
      fd = open(argv[i], O_RDONLY);
      if (fd < 0) {
        perror("Error opening file");
        return EXIT_FAILURE;
      }

      while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read) {
          perror("Error writing to stdout");
          close(fd);
          return EXIT_FAILURE;
        }
      }

      if (bytes_read < 0) {
        perror("Error reading file");
        close(fd);
        return EXIT_FAILURE;
      }

      close(fd);
    }
  }

  return EXIT_SUCCESS;
}
