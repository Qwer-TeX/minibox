#include "minibox.h"

/* common code for the cp and cat program */
/* cat [infile] */
/* cp [source] [destination] */
int cpcat(const char *strmin, const char *strmout) {
  FILE *finptr;
  FILE *foutptr;
  int chars;

  if (strmin == NULL || strcmp(strmin, "-") == 0) {
    finptr = stdin;
  } else {
    finptr = fopen(strmin, "r");
    if (finptr == NULL) {
      fprintf(stderr, "Error: Cannot open input file %s\n", strmin);
      return 1;
    }
  }

  if (strmout == NULL || strcmp(strmout, "-") == 0) {
    foutptr = stdout;
  } else {
    foutptr = fopen(strmout, "w");
    if (foutptr == NULL) {
      fprintf(stderr, "Error: Cannot open output file %s\n", strmout);
      if (finptr != stdin)
        fclose(finptr);
      return 1;
    }
  }

  while ((chars = fgetc(finptr)) != EOF) {
    fputc(chars, foutptr);
  }

  if (finptr != stdin)
    fclose(finptr);
  if (foutptr != stdout)
    fclose(foutptr);

  return 0;
}
