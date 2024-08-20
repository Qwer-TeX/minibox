#include "minibox.h"

static void print_usage() {
  printf("Usage: arch [-h]\n");
  printf("  -h      Display this help message\n");
}

int arch(int argc, char *argv[]) {
  if (argc > 2) {
    print_usage();
    return 1;
  }

  if (argc == 2) {
    if (argv[1][0] == '-' && argv[1][1] == 'h' && argv[1][2] == '\0') {
      print_usage();
      return 0;
    } else {
      print_usage();
      return 1;
    }
  }

  // Default behavior: print machine architecture
  struct utsname uname_data;
  if (uname(&uname_data) == 0) {
    printf("%s\n", uname_data.machine);
  } else {
    perror("uname");
    return 1;
  }

  return 0;
}
