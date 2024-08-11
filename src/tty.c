#include "minibox.h"

/* tty program */
int tty(void) {
  char *tty_path;
  int fd;

  // Get the path of the terminal device
  tty_path = ttyname(STDIN_FILENO);
  if (!tty_path) {
    fprintf(stderr, "Not a terminal.\n");
    return 1;
  }

  // Attempt to open the terminal device to ensure it's valid
  fd = open(tty_path, O_RDONLY);
  if (fd < 0) {
    perror("tty");
    return 1;
  }

  // Print the terminal path
  printf("%s\n", tty_path);

  // Close the file descriptor
  close(fd);

  return 0;
}
