#include "minibox.h"

/* nohup program */
int nohup(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <command> [arguments...]\n", argv[0]);
    return 1;
  }

  // Fork a child process
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    return 1;
  }

  if (pid == 0) {
    // In the child process

    // Ignore SIGHUP signal
    signal(SIGHUP, SIG_IGN);

    // Redirect stdout and stderr to a file
    freopen("nohup.out", "a", stdout);
    freopen("nohup.out", "a", stderr);

    // Execute the command
    execvp(argv[1], &argv[1]);

    // If execvp fails
    perror("execvp");
    exit(1);
  }

  // In the parent process
  // Wait for the child process to finish
  int status;
  waitpid(pid, &status, 0);

  return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}
