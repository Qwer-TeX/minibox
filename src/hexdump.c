#include "minibox.h"

/* hexdump program: display a file in hex */
int hexdump(int argc, char *argv[]) {
  FILE *fp;
  unsigned char buffer[16];
  size_t bytesRead;

  if (argc < 2) {
    fprintf(stderr, "Usage: hexdump <file>\n");
    return 1;
  }

  fp = fopen(argv[1], "rb");
  if (!fp) {
    perror("fopen");
    return 1;
  }

  while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
    for (size_t i = 0; i < bytesRead; i++) {
      printf("%02x ", buffer[i]);
    }
    printf("\n");
  }

  fclose(fp);
  return 0;
}
