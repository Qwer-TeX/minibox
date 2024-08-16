#include "minibox.h"

#define DEFAULT_TAB_WIDTH 8

int expand(int argc, char *argv[]) {
  FILE *input = stdin;
  FILE *output = stdout;
  int tab_width = DEFAULT_TAB_WIDTH;
  bool replace_tabs = false;
  bool replace_all_spaces = false;

  // Parse command-line arguments
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == 't') {
      tab_width = atoi(&argv[i][2]);
    } else if (argv[i][0] == '-' && argv[i][1] == 'i') {
      replace_tabs = true;
    } else if (argv[i][0] == '-' && argv[i][1] == 'a') {
      replace_all_spaces = true;
    } else {
      input = fopen(argv[i], "r");
      if (!input) {
        perror("Error opening input file");
        return 1;
      }
    }
  }

  int c;
  int column = 0;
  while ((c = fgetc(input)) != EOF) {
    if (replace_tabs && c == '\t') {
      int spaces = tab_width - (column % tab_width);
      for (int i = 0; i < spaces; i++)
        fputc(' ', output);
      column += spaces;
    } else if (replace_all_spaces && c == ' ') {
      int space_count = 1;
      while ((c = fgetc(input)) == ' ')
        space_count++;
      fseek(input, -1, SEEK_CUR); // Rewind one character
      int tabs = space_count / tab_width;
      int spaces = space_count % tab_width;
      for (int i = 0; i < tabs; i++)
        fputc('\t', output);
      for (int i = 0; i < spaces; i++)
        fputc(' ', output);
      column = (column + space_count) % tab_width;
    } else {
      if (c == '\n')
        column = 0;
      else
        column++;
      fputc(c, output);
    }
  }

  if (input != stdin)
    fclose(input);
  return 0;
}
