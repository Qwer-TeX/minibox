/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>.
 * All Rights Reserved.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 *
 * When adding programs or features, please consider if they can be
 * accomplished in a sane way with standard unix tools. If they're
 * programs or features you added, please make sure they are read-
 * able and understandable by a novice-advanced programmer, if not,
 * add comments or let me know. Use common sense and please don't
 * bloat sources.
 *
 * I haven't tested but it could compile on windows systems with MSYS/MinGW or
 * Cygwin. MiniBox should be fairly portable for POSIX systems.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

#include "minibox.h"

#define DEFAULT_LINES 10
#define DEFAULT_BYTES 1024

int tail(int argc, char *argv[]) {
  int lines = DEFAULT_LINES;
  int bytes = DEFAULT_BYTES;
  int follow = 0;
  int opt;

  while ((opt = getopt(argc, argv, "c:n:f")) != -1) {
    switch (opt) {
    case 'c':
      bytes = atoi(optarg);
      break;
    case 'n':
      lines = atoi(optarg);
      break;
    case 'f':
      follow = 1;
      break;
    default:
      fprintf(stderr, "Usage: %s [-c bytes] [-n lines] [-f] [file...]\n",
              argv[0]);
      return 1;
    }
  }

  if (optind >= argc) {
    fprintf(stderr, "No file specified\n");
    return 1;
  }

  for (int i = optind; i < argc; i++) {
    const char *filename = argv[i];
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
      perror("Error opening file");
      return 1;
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size < 0) {
      perror("Error seeking file");
      close(fd);
      return 1;
    }

    // For -c and -n options, we need to read from the end of the file
    if (bytes > 0) {
      off_t start = file_size > bytes ? file_size - bytes : 0;
      lseek(fd, start, SEEK_SET);
      char buffer[bytes];
      ssize_t n = read(fd, buffer, bytes);
      if (n < 0) {
        perror("Error reading file");
        close(fd);
        return 1;
      }
      write(STDOUT_FILENO, buffer, n);
    } else if (lines > 0) {
      // Read the last N lines
      char *buffer = malloc(file_size);
      if (!buffer) {
        perror("Memory allocation error");
        close(fd);
        return 1;
      }
      lseek(fd, 0, SEEK_SET);
      ssize_t n = read(fd, buffer, file_size);
      if (n < 0) {
        perror("Error reading file");
        free(buffer);
        close(fd);
        return 1;
      }

      int line_count = 0;
      for (ssize_t j = n - 1; j >= 0; j--) {
        if (buffer[j] == '\n') {
          line_count++;
          if (line_count >= lines) {
            write(STDOUT_FILENO, buffer + j + 1, n - j - 1);
            break;
          }
        }
      }
      free(buffer);
    }

    close(fd);

    if (follow) {
      // Follow mode: continuously display appended data
      fd = open(filename, O_RDONLY | O_NONBLOCK);
      if (fd < 0) {
        perror("Error reopening file in follow mode");
        return 1;
      }

      off_t last_size = lseek(fd, 0, SEEK_END);
      while (follow) {
        sleep(1);
        off_t current_size = lseek(fd, 0, SEEK_END);
        if (current_size > last_size) {
          lseek(fd, last_size, SEEK_SET);
          char buffer[current_size - last_size];
          ssize_t n = read(fd, buffer, current_size - last_size);
          if (n > 0) {
            write(STDOUT_FILENO, buffer, n);
          }
          last_size = current_size;
        }
      }
      close(fd);
    }
  }

  return 0;
}
