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

#define MAX_LINE_LENGTH 1024

// Function to print the usage of the command
static void print_usage(const char *prog_name) {
  fprintf(stderr, "Usage: %s [-d delimiter] [-s] file1 [file2 ...]\n",
          prog_name);
  fprintf(stderr,
          "  -d delimiter  Use the specified delimiter between columns.\n");
  fprintf(stderr,
          "  -s            Paste lines serially rather than side-by-side.\n");
}

// Function to paste files side-by-side or serially
int paste(int argc, char *argv[]) {
  int option;
  char delimiter = '\t'; // Default delimiter is tab
  int serial_mode = 0;   // Default is side-by-side mode

  // Parse command-line arguments
  while ((option = getopt(argc, argv, "d:s")) != -1) {
    switch (option) {
    case 'd':
      delimiter = optarg[0];
      break;
    case 's':
      serial_mode = 1;
      break;
    default:
      print_usage(argv[0]);
      return EXIT_FAILURE;
    }
  }

  // Check if there are file arguments
  if (optind >= argc) {
    fprintf(stderr, "Error: No files specified.\n");
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }

  FILE *files[argc - optind];
  char line[MAX_LINE_LENGTH];
  int end_of_file[argc - optind];
  int num_files = argc - optind;
  int i;

  // Open all files
  for (i = 0; i < num_files; i++) {
    files[i] = fopen(argv[optind + i], "r");
    if (files[i] == NULL) {
      perror("fopen");
      // Close already opened files before exiting
      for (int j = 0; j < i; j++) {
        fclose(files[j]);
      }
      return EXIT_FAILURE;
    }
    end_of_file[i] = 0;
  }

  if (serial_mode) {
    // Serial mode: Concatenate lines from files
    while (1) {
      int all_files_done = 1;

      for (i = 0; i < num_files; i++) {
        if (!end_of_file[i]) {
          if (fgets(line, sizeof(line), files[i]) != NULL) {
            printf("%s", line);
            all_files_done = 0;
          } else {
            end_of_file[i] = 1;
          }
        }
      }

      if (all_files_done) {
        break;
      }
    }
  } else {
    // Side-by-side mode: Output lines from files side-by-side
    while (1) {
      int all_files_done = 1;

      for (i = 0; i < num_files; i++) {
        if (!end_of_file[i]) {
          if (fgets(line, sizeof(line), files[i]) != NULL) {
            all_files_done = 0;
            printf("%s", line);
            if (i < num_files - 1) {
              printf("%c", delimiter);
            }
          } else {
            end_of_file[i] = 1;
          }
        }
      }

      if (all_files_done) {
        break;
      }

      printf("\n");
    }
  }

  // Close all files
  for (i = 0; i < num_files; i++) {
    fclose(files[i]);
  }

  return EXIT_SUCCESS;
}
