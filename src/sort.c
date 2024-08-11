#include "minibox.h"

// Comparison function for qsort
int compare_lines(const void *a, const void *b) {
  return strcmp(*(const char **)a, *(const char **)b);
}

/* sort program */
int sort(int argc, char *argv[]) {
  char *lines[1000]; // Array to store lines (simple fixed-size buffer)
  size_t count = 0;
  size_t capacity = sizeof(lines) / sizeof(lines[0]);

  // Read lines from standard input or file if provided
  FILE *file = stdin;
  if (argc == 2) {
    file = fopen(argv[1], "r");
    if (!file) {
      perror("Error opening file");
      return EXIT_FAILURE;
    }
  }

  char buffer[1024];
  while (fgets(buffer, sizeof(buffer), file)) {
    if (count >= capacity) {
      fprintf(stderr, "Too many lines to sort\n");
      return EXIT_FAILURE;
    }
    // Remove newline character and allocate memory for line
    buffer[strcspn(buffer, "\n")] = '\0';
    lines[count] = strdup(buffer);
    if (!lines[count]) {
      perror("Memory allocation failed");
      return EXIT_FAILURE;
    }
    count++;
  }

  if (file != stdin) {
    fclose(file);
  }

  // Sort the lines
  qsort(lines, count, sizeof(char *), compare_lines);

  // Print the sorted lines
  for (size_t i = 0; i < count; i++) {
    puts(lines[i]);
    free(lines[i]);
  }

  return EXIT_SUCCESS;
}
