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
