/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>.
 * All Rights Reserved.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 * minibox-shell.c is the main shell source with all shell builtins.
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

/* XXX: Writing a shell is a very complex task, though I don't have time to
 * write one. Contributions of a shell are welcome as they give me a
 * half-complete structure to work on instead of deciding where to start. Or if
 * someone could fork the ash sources from busybox so that we could try to
 * simplify the source code resulting in a ash shell that I could implement,
 * that would be excellent. We could create the very stripped-down version of
 * the busybox ash which would come to be known as the minibox shell. Compiling
 * busybox with allnoconfig and menuconfig it to enable ash and its features
 * would result in a 88k executable. It could become probably 14k to 48k if we
 * were to fork and strip it down ;). Or as usual, get started at creating our
 * own shell from scratch but that would be off the limits for me since I don't
 * have enough time to do it all though I can create examples since they're
 * simple and effective.
 *
 *      The minibox shell has to be implemented in a seperate file and directory
 * because I know that the lines of code for this shell would be atleast 600
 * lines with comments included. Remember its the stripped-down version of the
 * busybox ash so its has to not exceede 600-1000 lines of code. But the example
 * shells are less than 200 lines of code so I thought maybe we can make our
 * shell ultra small but remember those example shells are only stripped down
 * and don't yet have implemented its shell language functions like if, case,
 * else, until, for, test, etc. If someone can contribute codes for those
 * functions, I would be much more than thankful.
 *
 *      No its not going to be seperate from the minibox.c since minibox.o and
 * minibox- shell.o are going to be linked together because this projects's goal
 * is for a multi-call binary but I'll give two options you can adjust in the
 * Makefile for creating a standalone shell or including it in the multi-call
 * binary.
 */

#include "linenoise/linenoise.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_TOKENS 64
#define TOKEN_DELIMITERS " \t\r\n\a"

// Function to evaluate arithmetic expressions in $(())
int evaluate_arithmetic_expression(const char *expression) {
  int result = 0;
  sscanf(expression, "%d", &result);
  return result;
}

// Function to expand variables prefixed with '$'
char *expand_variable(char *token) {
  char *expanded;
  if (token[0] == '$') {
    if (token[1] == '{') {
      // Handle ${VAR} syntax
      char *end_brace = strchr(token, '}');
      if (end_brace) {
        char var_name[64];
        strncpy(var_name, token + 2, end_brace - token - 2);
        var_name[end_brace - token - 2] = '\0';
        char *value = getenv(var_name);
        if (value) {
          expanded = malloc(strlen(value) + 1);
          if (!expanded) {
            fprintf(stderr, "allocation error\n");
            exit(EXIT_FAILURE);
          }
          strcpy(expanded, value);
          return expanded;
        } else {
          fprintf(stderr, "Variable '%s' not set\n", var_name);
          return NULL;
        }
      } else {
        fprintf(stderr, "Unmatched '{' in variable expansion\n");
        return NULL;
      }
    } else if (token[1] == '(' && token[2] == '(') {
      // Handle $((...)) syntax
      char *end_paren = strstr(token, "))");
      if (end_paren) {
        char expression[64];
        strncpy(expression, token + 3, end_paren - token - 3);
        expression[end_paren - token - 3] = '\0';
        int result = evaluate_arithmetic_expression(expression);
        char result_str[16];
        snprintf(result_str, sizeof(result_str), "%d", result);
        expanded = malloc(strlen(result_str) + 1);
        if (!expanded) {
          fprintf(stderr, "allocation error\n");
          exit(EXIT_FAILURE);
        }
        strcpy(expanded, result_str);
        return expanded;
      } else {
        fprintf(stderr, "Unmatched '((' in arithmetic expansion\n");
        return NULL;
      }
    } else {
      // Handle $VAR syntax
      char *var_name = token + 1;
      char *value = getenv(var_name);
      if (value != NULL) {
        expanded = malloc(strlen(value) + 1);
        if (!expanded) {
          fprintf(stderr, "allocation error\n");
          exit(EXIT_FAILURE);
        }
        strcpy(expanded, value);
        return expanded;
      } else {
        fprintf(stderr, "Variable '%s' not set\n", var_name);
        return NULL;
      }
    }
  } else {
    expanded = malloc(strlen(token) + 1);
    if (!expanded) {
      fprintf(stderr, "allocation error\n");
      exit(EXIT_FAILURE);
    }
    strcpy(expanded, token);
    return expanded;
  }
}

// Function to split a line into tokens (arguments)
char **split_line(char *line) {
  int bufsize = MAX_TOKENS, position = 0;
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token, *expanded_token;

  if (!tokens) {
    fprintf(stderr, "allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOKEN_DELIMITERS);
  while (token != NULL) {
    expanded_token = expand_variable(token);
    if (expanded_token != NULL) {
      tokens[position] = expanded_token;
      position++;
    }

    if (position >= bufsize) {
      bufsize += MAX_TOKENS;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      if (!tokens) {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOKEN_DELIMITERS);
  }
  tokens[position] = NULL;
  return tokens;
}

// Built-in command: exit
int shell_exit(char **args) { return 0; }

// Built-in command: cd
int shell_cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("mbsh");
    }
  }
  return 1;
}

// Built-in command: export
int shell_export(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "expected argument to \"export\"\n");
  } else {
    char *eq_sign = strchr(args[1], '=');
    if (eq_sign) {
      *eq_sign = '\0';
      char *var = args[1];
      char *val = eq_sign + 1;
      if (setenv(var, val, 1) != 0) {
        perror("export");
      }
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
    if ((arg[0] == '"' && arg[len - 1] == '"') ||
        (arg[0] == '\'' && arg[len - 1] == '\'')) {
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
char *builtin_str[] = {"cd", "exit", "export", "history", "echo"};

// Corresponding functions for built-in commands
int (*builtin_func[])(char **) = {&shell_cd, &shell_exit, &shell_export,
                                  &shell_history, &shell_echo};

// Function to execute built-in commands
int execute_builtin(char **args) {
  for (int i = 0; i < sizeof(builtin_str) / sizeof(char *); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  return -1; // Not a built-in command
}

// Function to handle variable assignment
int handle_variable_assignment(char *arg) {
  char *eq_sign = strchr(arg, '=');
  if (eq_sign) {
    *eq_sign = '\0';
    char *var = arg;
    char *val = eq_sign + 1;
    if (setenv(var, val, 1) != 0) {
      perror("setenv");
    }
    return 1; // Variable assignment handled
  }
  return 0; // Not a variable assignment
}

// Function to execute a command
int execute_command(char **args) {
  if (args[0] == NULL) {
    // An empty command was entered
    return 1;
  }

  if (handle_variable_assignment(args[0])) {
    return 1; // Handled as variable assignment
  }

  int builtin_status = execute_builtin(args);
  if (builtin_status != -1) {
    return builtin_status;
  }

  return launch_program(args);
}

// Function to launch a program
int launch_program(char **args) {
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("mbsh");
      exit(EXIT_FAILURE);
    }
  } else if (pid < 0) {
    // Error forking
    perror("mbsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
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
