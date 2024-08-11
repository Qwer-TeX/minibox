#include "minibox.h"

/* free program */
/* Display the amount of free and used memory in the system */
/* FIXME: free displays 0 for shared while coreutils free displays non-zero */
int free_cmd(void) {
  FILE *file = fopen("/proc/meminfo", "r");
  if (!file) {
    perror("fopen");
    return 1;
  }

  char line[256];
  unsigned long total = 0, free = 0, available = 0, buffers = 0, cached = 0;

  while (fgets(line, sizeof(line), file)) {
    if (sscanf(line, "MemTotal: %lu kB", &total) == 1 ||
        sscanf(line, "MemFree: %lu kB", &free) == 1 ||
        sscanf(line, "MemAvailable: %lu kB", &available) == 1 ||
        sscanf(line, "Buffers: %lu kB", &buffers) == 1 ||
        sscanf(line, "Cached: %lu kB", &cached) == 1) {
      // Parsed a line
    }
  }

  fclose(file);

  unsigned long used = total - free - buffers - cached;

  printf("            total       used       free     shared  buff/cache  "
         "available\n");
  printf("Mem:   %10lu %10lu %10lu %10lu %10lu %10lu\n", total, used, free, 0UL,
         buffers + cached, available);

  return 0;
}
