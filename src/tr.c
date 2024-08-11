#include "minibox.h"

/* tr program */
int tr(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s SET1 SET2\n", argv[0]);
    return EXIT_FAILURE;
  }

  const char *set1 = argv[1];
  const char *set2 = argv[2];
  if (strlen(set1) != strlen(set2)) {
    fprintf(stderr, "SET1 and SET2 must be of the same length.\n");
    return EXIT_FAILURE;
  }

  // Create a translation table
  int translate[256];
  for (int i = 0; i < 256; ++i) {
    translate[i] = i; // Default to no translation
  }

  for (size_t i = 0; i < strlen(set1); ++i) {
    translate[(unsigned char)set1[i]] = (unsigned char)set2[i];
  }

  // Read from standard input (or file if provided)
  int c;
  while ((c = getchar()) != EOF) {
    putchar(translate[c]);
  }

  return EXIT_SUCCESS;
}
