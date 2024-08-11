#include "minibox.h"

/* w program */
/* Display logged-in users and their activities */
/* FIXME: Output is broken and doesn't display info in their correct respective
 * columns */
int w(void) {
  struct sysinfo si;
  struct utmp *ut;
  struct tm *tm;
  time_t now;
  FILE *fp;
  char buf[256];

  // Get system uptime
  if (sysinfo(&si) < 0) {
    perror("sysinfo");
    return 1;
  }
  now = time(NULL);

  // Print header
  printf(" %s up ", strtok(ctime(&now), "\n"));
  printf("%ld users,  load average: %.2f, %.2f, %.2f\n",
         (long)sysconf(_SC_NPROCESSORS_ONLN), (double)si.loads[0] / 65536.0,
         (double)si.loads[1] / 65536.0, (double)si.loads[2] / 65536.0);
  printf(
      "USER     TTY      FROM              LOGIN@   IDLE   JCPU   PCPU WHAT\n");

  // Read and display user information
  setutent();
  while ((ut = getutent())) {
    if (ut->ut_type == USER_PROCESS) {
      tm = localtime(&ut->ut_tv.tv_sec);
      strftime(buf, sizeof(buf), "%H:%M", tm);

      printf("%-8s %-8s %-16s %s  ", ut->ut_user, ut->ut_line,
             ut->ut_host[0] != '\0' ? ut->ut_host : "-", buf);

      // Calculate idle time
      snprintf(buf, sizeof(buf), "/proc/%d/stat", ut->ut_pid);
      if ((fp = fopen(buf, "r"))) {
        long unsigned int starttime, utime, stime;
        if (fscanf(fp,
                   "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu "
                   "%lu %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*u %lu",
                   &utime, &stime, &starttime) == 3) {
          fclose(fp);
          printf("%-6s ", "IDLE");
          printf("%-6s ", "JCPU");
          printf("%-6s ", "PCPU");
          printf("%s\n", ut->ut_line);
        } else {
          fclose(fp);
        }
      }
    }
  }
  endutent();
  return 0;
}
