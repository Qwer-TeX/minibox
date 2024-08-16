#include "minibox.h"

// I remember I made a similar program like this to aid in learning C, that
// functionality I added here. I didn't know that this was a standard unix
// program. A example of this program was in a C programming reference book
// without argument parsing, I enhanced it in my own way and tried to make it
// as compatible
int factor(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s number...\n", argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    int num = atoi(argv[i]);
    if (num <= 0) {
      fprintf(stderr, "Invalid number: %s\n", argv[i]);
      return 1;
    }

    if (num < 2) {
      printf("%d\n", num);
      continue;
    }

    printf("%d:", num);
    int original_n = num;
    int divisor = 2;

    while (num > 1) {
      if (num % divisor == 0) {
        printf(" %d", divisor);
        num /= divisor;
      } else {
        divisor++;
      }
    }
    printf("\n");
  }

  return 0;
}
