#include "minibox.h"

// Function to print the usage of the command
// not defined in utils.h since it is local to this program only
static void print_usage(const char *prog_name) {
  fprintf(stderr, "Usage: %s [-c num] [-n num] [file]\n", prog_name);
  fprintf(stderr, "  -c num  Output the first `num` characters.\n");
  fprintf(stderr, "  -n num  Output the first `num` lines.\n");
  fprintf(
      stderr,
      "  file     File to read; if not provided, reads from standard input.\n");
}

int head(int argc, char *argv[]) {
  int num_lines = 10; // Default number of lines
  int num_chars = -1; // Default: no limit on number of characters
  int option;

  // Parse command-line arguments
  while ((option = getopt(argc, argv, "c:n:")) != -1) {
    switch (option) {
    case 'c':
      num_chars = atoi(optarg);
      if (num_chars < 0) {
        fprintf(stderr, "Invalid number of characters: %d\n", num_chars);
        return EXIT_FAILURE;
      }
      break;
    case 'n':
      num_lines = atoi(optarg);
      if (num_lines < 0) {
        fprintf(stderr, "Invalid number of lines: %d\n", num_lines);
        return EXIT_FAILURE;
      }
      break;
    default:
      print_usage(argv[0]);
      return EXIT_FAILURE;
    }
  }

  // If there are file arguments, open the first one
  FILE *input = stdin; // Default to standard input
  if (optind < argc) {
    input = fopen(argv[optind], "r");
    if (input == NULL) {
      perror("fopen");
      return EXIT_FAILURE;
    }
  }

  // Read and output the desired content
  if (num_chars >= 0) {
    // Output the first `num_chars` characters
    int c;
    int count = 0;
    while ((c = fgetc(input)) != EOF && (num_chars < 0 || count < num_chars)) {
      putchar(c);
      count++;
    }
  } else {
    // Output the first `num_lines` lines
    char buffer[BUFSIZ];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), input) != NULL &&
           (num_lines < 0 || count < num_lines)) {
      fputs(buffer, stdout);
      count++;
    }
  }

  if (input != stdin) {
    fclose(input);
  }

  return EXIT_SUCCESS;
}
