#include "minibox.h"

/* wc program */
/* Usage: wc [<] [infile] */
int wc(FILE *strmin, FILE *strmout) {
  int letter, nline, nchar, nword, inword;

  inword = 0;
  nline = nword = nchar = 0;
  while ((letter = getc(strmin)) != EOF) {
    ++nchar;
    if (letter == '\n')
      ++nline;
    if (letter == ' ' || letter == '\n' || letter == '\t')
      inword = 1;
    else if (inword == 0) {
      inword = 1;
      ++nword;
    }
  }
  fprintf(strmout, "%d  %d  %d\n", nline, nword, nchar);
  return 0;
}
