/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>.
 * All Rights Reserved.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 * minibox-shell.c is the main shell source with all shell builtins.
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

/* cut program */
int cut(int argc, char *argv[]) {
  char delimiter = '\t';  // Default delimiter (tab)
  char *field_str = NULL; // Field string
  int opt;
  size_t num_fields = 0;
  int *fields = NULL;

  // Parse command-line options
  while ((opt = getopt(argc, argv, "d:f:")) != -1) {
    switch (opt) {
    case 'd':
      delimiter = optarg[0];
      break;
    case 'f':
      field_str = optarg;
      break;
    default:
      fprintf(stderr, "Usage: %s [-d delimiter] -f fields [file...]\n",
              argv[0]);
      return EXIT_FAILURE;
    }
  }

  if (!field_str) {
    fprintf(stderr, "Field specification is required.\n");
    return EXIT_FAILURE;
  }

  // Parse the field argument
  size_t fields_capacity = 10;
  fields = malloc(fields_capacity * sizeof(int));
  if (!fields) {
    perror("malloc");
    return EXIT_FAILURE;
  }

  char *str = strdup(field_str);
  if (!str) {
    perror("strdup");
    free(fields);
    return EXIT_FAILURE;
  }

  char *token = strtok(str, ",");
  while (token != NULL) {
    if (num_fields >= fields_capacity) {
      fields_capacity *= 2;
      fields = realloc(fields, fields_capacity * sizeof(int));
      if (!fields) {
        perror("realloc");
        free(str);
        return EXIT_FAILURE;
      }
    }
    fields[num_fields++] = atoi(token);
    token = strtok(NULL, ",");
  }
  free(str);

  // Process lines from stdin or files
  if (optind == argc) {
    // No files specified, read from stdin
    char line[1024];
    while (fgets(line, sizeof(line), stdin)) {
      char *temp_line = strdup(line);
      if (!temp_line) {
        perror("strdup");
        free(fields);
        return EXIT_FAILURE;
      }

      char *token = strtok(temp_line, &delimiter);
      int current_field = 1;
      size_t field_index = 0;

      while (token != NULL) {
        while (field_index < num_fields &&
               fields[field_index] < current_field) {
          field_index++;
        }
        if (field_index < num_fields && fields[field_index] == current_field) {
          printf("%s\n", token);
        }
        current_field++;
        token = strtok(NULL, &delimiter);
      }

      free(temp_line);
    }
  } else {
    // Process each file
    for (int i = optind; i < argc; i++) {
      FILE *file = fopen(argv[i], "r");
      if (!file) {
        perror(argv[i]);
        continue;
      }

      char line[1024];
      while (fgets(line, sizeof(line), file)) {
        char *temp_line = strdup(line);
        if (!temp_line) {
          perror("strdup");
          fclose(file);
          free(fields);
          return EXIT_FAILURE;
        }

        char *token = strtok(temp_line, &delimiter);
        int current_field = 1;
        size_t field_index = 0;

        while (token != NULL) {
          while (field_index < num_fields &&
                 fields[field_index] < current_field) {
            field_index++;
          }
          if (field_index < num_fields &&
              fields[field_index] == current_field) {
            printf("%s\n", token);
          }
          current_field++;
          token = strtok(NULL, &delimiter);
        }

        free(temp_line);
      }

      fclose(file);
    }
  }

  free(fields);
  return EXIT_SUCCESS;
}
