#include "minibox.h"

/* cmp program */
/* Usage: cmp [comparer] [comparent] */
int cmp(const char *file1, const char *file2) {
  FILE *f1 = fopen(file1, "r");
  FILE *f2 = fopen(file2, "r");
  int ch1, ch2;

  if (!f1 || !f2) {
    fprintf(stderr, "cmp: Cannot open file\n");
    return 2;
  }

  while ((ch1 = fgetc(f1)) != EOF && (ch2 = fgetc(f2)) != EOF) {
    if (ch1 != ch2) {
      printf("Files differ\n");
      fclose(f1);
      fclose(f2);
      return 1;
    }
  }

  if ((ch1 != EOF) || (ch2 != EOF)) {
    printf("Files differ\n");
    fclose(f1);
    fclose(f2);
    return 1;
  }

  fclose(f1);
  fclose(f2);
  return 0;
}
