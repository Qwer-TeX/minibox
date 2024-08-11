#include "minibox.h"

/* grep program */
int grep(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <pattern> <file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  const char *pattern = argv[1];
  const char *filename = argv[2];
  FILE *file = fopen(filename, "r");

  if (!file) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  char line[1024]; // Buffer to store each line read from the file
  while (fgets(line, sizeof(line), file)) {
    if (strstr(line, pattern)) { // Check if the pattern is in the current line
      printf("%s", line);        // Print the matching line
    }
  }

  fclose(file);
  return EXIT_SUCCESS;
}
