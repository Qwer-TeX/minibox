#include "minibox.h"

#define BUFFER_SIZE 1024

int cp(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  int src_fd = open(argv[1], O_RDONLY);
  if (src_fd < 0) {
    perror("Error opening source file");
    return EXIT_FAILURE;
  }

  int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (dest_fd < 0) {
    perror("Error opening destination file");
    close(src_fd);
    return EXIT_FAILURE;
  }

  char buffer[BUFFER_SIZE];
  ssize_t bytes_read, bytes_written;

  while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
    bytes_written = write(dest_fd, buffer, bytes_read);
    if (bytes_written != bytes_read) {
      perror("Error writing to destination file");
      close(src_fd);
      close(dest_fd);
      return EXIT_FAILURE;
    }
  }

  if (bytes_read < 0) {
    perror("Error reading source file");
  }

  close(src_fd);
  close(dest_fd);

  return (bytes_read < 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
