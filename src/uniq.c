#include "minibox.h"

/* uniq program */
int uniq(int argc, char *argv[]) {
  char *prev_line = NULL;
  char buffer[1024];

  // Read lines from standard input or file if provided
  FILE *file = stdin;
  if (argc == 2) {
    file = fopen(argv[1], "r");
    if (!file) {
      perror("Error opening file");
      return EXIT_FAILURE;
    }
  }

  while (fgets(buffer, sizeof(buffer), file)) {
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

    if (prev_line == NULL || strcmp(prev_line, buffer) != 0) {
      if (prev_line != NULL) {
        puts(prev_line);
        free(prev_line);
      }
      prev_line = strdup(buffer);
      if (!prev_line) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
      }
    }
  }

  if (prev_line != NULL) {
    puts(prev_line);
    free(prev_line);
  }

  if (file != stdin) {
    fclose(file);
  }

  return EXIT_SUCCESS;
}
