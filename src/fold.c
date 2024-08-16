#include "minibox.h"

#define DEFAULT_WIDTH 80

int fold(int argc, char *argv[]) {
  FILE *input = stdin;
  FILE *output = stdout;
  int width = DEFAULT_WIDTH;
  bool break_at_spaces = false;
  bool break_at_bytes = false;

  // Parse command-line arguments
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == 'w') {
      width = atoi(&argv[i][2]);
    } else if (argv[i][0] == '-' && argv[i][1] == 's') {
      break_at_spaces = true;
    } else if (argv[i][0] == '-' && argv[i][1] == 'b') {
      break_at_bytes = true;
    } else {
      input = fopen(argv[i], "r");
      if (!input) {
        perror("Error opening input file");
        return 1;
      }
    }
  }

  int c, column = 0, last_space = -1;

  while ((c = fgetc(input)) != EOF) {
    if (break_at_bytes) {
      if (column >= width) {
        fputc('\n', output);
        column = 0;
        last_space = -1;
      }
    } else {
      if (column >= width) {
        if (break_at_spaces && last_space != -1) {
          fseek(input, last_space, SEEK_SET);
          fputc('\n', output);
          column = 0;
          last_space = -1;
        } else {
          fputc('\n', output);
          column = 0;
        }
      }
    }

    if (c == ' ') {
      last_space = ftell(input);
    }

    fputc(c, output);
    column++;

    if (c == '\n') {
      column = 0;
      last_space = -1;
    }
  }

  if (column > 0 && !break_at_bytes && break_at_spaces && last_space != -1) {
    fseek(input, last_space, SEEK_SET);
    fputc('\n', output);
  }

  if (input != stdin) {
    fclose(input);
  }

  return 0;
}
