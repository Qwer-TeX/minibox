/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>.
 * All Rights Reserved.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 *
 * When adding programs or features, please consider if they can be
 * accomplished in a sane way with standard unix tools. If they're
 * programs or features you added, please make sure they are read-
 * able and understandable by a novice-advanced programmer, if not,
 * add comments or let me know. Use common sense and please don't
 * bloat sources.
 *
 * I haven't tested but it could compile on windows systems with MSYS/MinGW or
 * Cygwin. MiniBox should be fairly portable for POSIX systems.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

#include "minibox.h"

#define INITIAL_SIZE 10

extern char **environ;

int env(int argc, char *argv[]) {
  int ignore_env = 0;
  char *remove_var = NULL;
  char *command = NULL;
  char **cmd_args = NULL;
  int cmd_arg_count = 0;
  int cmd_arg_size = INITIAL_SIZE;

  // Allocate initial memory for command arguments
  cmd_args = malloc(cmd_arg_size * sizeof(char *));
  if (cmd_args == NULL) {
    perror("malloc");
    return EXIT_FAILURE;
  }

  // Parse command-line arguments
  int i = 1;
  while (i < argc) {
    if (strcmp(argv[i], "-i") == 0) {
      ignore_env = 1;
    } else if (strcmp(argv[i], "-u") == 0) {
      if (++i < argc) {
        remove_var = argv[i];
      } else {
        fprintf(stderr, "env: option requires an argument -- 'u'\n");
        fprintf(stderr, "Usage: %s [-i] [-u NAME] [COMMAND [ARGUMENTS ...]]\n",
                argv[0]);
        free(cmd_args);
        return EXIT_FAILURE;
      }
    } else {
      break;
    }
    i++;
  }

  // Check if a command is provided
  if (i < argc) {
    command = argv[i++];
  } else if (ignore_env || remove_var) {
    fprintf(stderr, "env: no command specified\n");
    fprintf(stderr, "Usage: %s [-i] [-u NAME] [COMMAND [ARGUMENTS ...]]\n",
            argv[0]);
    free(cmd_args);
    return EXIT_FAILURE;
  }

  // Collect command arguments
  while (i < argc) {
    if (cmd_arg_count >= cmd_arg_size - 1) {
      cmd_arg_size *= 2;
      char **new_cmd_args = realloc(cmd_args, cmd_arg_size * sizeof(char *));
      if (new_cmd_args == NULL) {
        perror("realloc");
        free(cmd_args);
        return EXIT_FAILURE;
      }
      cmd_args = new_cmd_args;
    }
    cmd_args[cmd_arg_count++] = argv[i++];
  }
  cmd_args[cmd_arg_count] = NULL;

  // Prepare environment for execve
  char **envp;
  if (ignore_env) {
    envp = NULL;
  } else {
    int env_count = 0;
    while (environ[env_count] != NULL) {
      env_count++;
    }

    envp = malloc((env_count + 1) * sizeof(char *));
    if (envp == NULL) {
      perror("malloc");
      free(cmd_args);
      return EXIT_FAILURE;
    }

    // Copy environment
    for (int j = 0; j < env_count; j++) {
      envp[j] = strdup(environ[j]);
      if (envp[j] == NULL) {
        perror("strdup");
        for (int k = 0; k < j; k++) {
          free(envp[k]);
        }
        free(envp);
        free(cmd_args);
        return EXIT_FAILURE;
      }
    }
    envp[env_count] = NULL;

    // Remove specified environment variable if -u was used
    if (remove_var) {
      int index = 0;
      while (envp[index] != NULL) {
        if (strncmp(envp[index], remove_var, strlen(remove_var)) == 0 &&
            envp[index][strlen(remove_var)] == '=') {
          free(envp[index]);
          while (envp[index] != NULL) {
            envp[index] = envp[index + 1];
            index++;
          }
          break;
        }
        index++;
      }
    }
  }

  // Execute the command
  if (command) {
    execve(command, cmd_args, envp);
    perror("execve");
    // Free allocated memory
    if (envp != NULL) {
      int index = 0;
      while (envp[index] != NULL) {
        free(envp[index]);
        index++;
      }
      free(envp);
    }
    free(cmd_args);
    return EXIT_FAILURE;
  } else {
    // Print the environment variables
    for (int i = 0; environ[i] != NULL; i++) {
      printf("%s\n", environ[i]);
    }
  }

  free(cmd_args);
  return EXIT_SUCCESS;
}
