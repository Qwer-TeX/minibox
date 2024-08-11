#include "minibox.h"

/* uptime program */
int uptime() {
  struct sysinfo info;

  if (sysinfo(&info) != 0) {
    perror("sysinfo");
    return EXIT_FAILURE;
  }

  // Calculate uptime in days, hours, minutes, and seconds
  long uptime_seconds = info.uptime;
  long days = uptime_seconds / (24 * 3600);
  uptime_seconds %= (24 * 3600);
  long hours = uptime_seconds / 3600;
  uptime_seconds %= 3600;
  long minutes = uptime_seconds / 60;
  long seconds = uptime_seconds % 60;

  // Print uptime
  printf("uptime: %ld day(s), %ld:%02ld:%02ld\n", days, hours, minutes,
         seconds);

  return EXIT_SUCCESS;
}
