/*
 *
 *  gcc -s -Os -Ilinenoise -o example3 example3.c linenoise/linenoise.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "linenoise.h"

// Function to expand variables prefixed with '$'
char *expand_variable(char *token) {
  if (token[0] == '$') {
    char *var_name = token + 1; // Skip the '$'
    char *value = getenv(var_name);
    if (value != NULL) {
      return strdup(value); // Duplicate the value since getenv returns a pointer to an internal buffer
    } else {
      fprintf(stderr, "Variable '%s' not set\n", var_name);
      return NULL;
    }
  } else {
    return strdup(token); // No expansion needed
  }
}

// Function to split a line into tokens (arguments)
char **split_line(char *line) {
  int bufsize = 64, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, *expanded_token;

  if (!tokens) {
    fprintf(stderr, "allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, " \t\r\n\a");
  while (token != NULL) {
    expanded_token = expand_variable(token);
    if (expanded_token != NULL) {
      tokens[position] = expanded_token;
      position++;
    }

    if (position >= bufsize) {
      bufsize += 64;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, " \t\r\n\a");
  }
  tokens[position] = NULL;
  return tokens;
}

// Built-in command: exit
int shell_exit(char **args) {
  return 0;
}

// Built-in command: cd
int shell_cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("shell");
    }
  }
  return 1;
}

// Built-in command: export
int shell_export(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "expected argument to \"export\"\n");
  } else {
    char *var = strtok(args[1], "=");
    char *val = strtok(NULL, "=");
    if (val) {
      setenv(var, val, 1);
    } else {
      fprintf(stderr, "export: invalid format\n");
    }
  }
  return 1;
}

// Built-in command: history
int shell_history(char **args) {
  // Implement history command if needed
  return 1;
}

// Built-in command: echo
int shell_echo(char **args) {
  for (int i = 1; args[i] != NULL; i++) {
    char *arg = args[i];
    int len = strlen(arg);

    // Check if the argument starts and ends with quotes
    if ((arg[0] == '"' && arg[len - 1] == '"') || (arg[0] == '\'' && arg[len - 1] == '\'')) {
      // Print the argument without the outer quotes
      printf("%.*s ", len - 2, arg + 1);
    } else {
      // Print the argument as is
      printf("%s ", arg);
    }
  }
  printf("\n");
  return 1;
}

// List of built-in commands
char *builtin_str[] = {
  "cd",
  "exit",
  "export",
  "history",
  "echo" // Added echo to the list of built-in commands
};

// Corresponding functions for built-in commands
int (*builtin_func[]) (char **) = {
  &shell_cd,
  &shell_exit,
  &shell_export,
  &shell_history,
  &shell_echo // Added shell_echo function pointer
};

// Function to execute built-in commands
int execute_builtin(char **args) {
  for (int i = 0; i < sizeof(builtin_str) / sizeof(char *); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  return -1; // Not a built-in command
}

// Function to launch a program
int launch_program(char **args) {
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("shell");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("shell");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

// Function to execute a command
int execute_command(char **args) {
  if (args[0] == NULL) {
    // An empty command was entered
    return 1;
  }

  int builtin_status = execute_builtin(args);
  if (builtin_status != -1) {
    return builtin_status;
  }

  return launch_program(args);
}

// Main loop
void shell_loop() {
  char *line;
  char **args;
  int status;

  linenoiseHistorySetMaxLen(100);

  do {
    line = linenoise("$ ");
    if (line == NULL) {
      break;
    }

    linenoiseHistoryAdd(line);

    args = split_line(line);
    status = execute_command(args);

    free(line);
    free(args);
  } while (status);
}

// Main function
int main(int argc, char **argv) {
  // Run command loop.
  shell_loop();

  return EXIT_SUCCESS;
}
