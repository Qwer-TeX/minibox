#include "minibox.h"

/* ps program */
int ps() {
  DIR *proc_dir = opendir("/proc");
  if (proc_dir == NULL) {
    perror("opendir");
    return EXIT_FAILURE;
  }

  struct dirent *entry;
  while ((entry = readdir(proc_dir)) != NULL) {
    // Check if the directory name is a number (i.e., a PID)
    if (entry->d_type == DT_DIR && atoi(entry->d_name) > 0) {
      char path[256];
      snprintf(path, sizeof(path), "/proc/%s/comm", entry->d_name);

      FILE *comm_file = fopen(path, "r");
      if (comm_file) {
        char process_name[256];
        if (fgets(process_name, sizeof(process_name), comm_file)) {
          // Remove the newline character at the end
          process_name[strcspn(process_name, "\n")] = '\0';
          printf("%d\t%s\n", atoi(entry->d_name), process_name);
        }
        fclose(comm_file);
      } else {
        // If the comm file cannot be opened, print an error message
        fprintf(stderr, "Warning: Could not open %s: %s\n", path,
                strerror(errno));
      }
    }
  }

  closedir(proc_dir);
  return EXIT_SUCCESS;
}
