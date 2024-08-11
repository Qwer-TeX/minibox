#include "minibox.h"

/* od program: octal dump */
/* FIXME: First output column should be in octal not hexadecimal */
int od(int argc, char *argv[]) {
  FILE *fp;
  unsigned char buffer[16];
  size_t bytesRead;

  if (argc < 2) {
    fprintf(stderr, "Usage: od <file>\n");
    return 1;
  }

  fp = fopen(argv[1], "rb");
  if (!fp) {
    perror("fopen");
    return 1;
  }

  while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
    printf("%08zx: ", (unsigned int)ftell(fp) - bytesRead);
    for (size_t i = 0; i < bytesRead; i++) {
      printf("%02x ", buffer[i]);
    }
    for (size_t i = bytesRead; i < sizeof(buffer); i++) {
      printf("   ");
    }
    printf("\n");
  }

  fclose(fp);
  return 0;
}
