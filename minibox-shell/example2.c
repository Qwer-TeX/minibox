/*
 *
 * gcc -o example2 example2.c -l:libreadline.a -ltinfo -lc -Wl,-Bdynamic
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

// Function to expand environment variables
char *expand_vars(char *line) {
  char *result = malloc(strlen(line) + 1);
  if (!result) {
    fprintf(stderr, "allocation error\n");
    exit(EXIT_FAILURE);
  }
  char *res_ptr = result;
  char *ptr = line;

  while (*ptr) {
    if (*ptr == '$' && *(ptr + 1) != ' ' && *(ptr + 1) != '\0') {
      ptr++;
      char varname[64] = {0};
      int var_len = 0;

      while (*ptr && (*ptr != ' ') && (*ptr != '$') && var_len < sizeof(varname) - 1) {
        varname[var_len++] = *ptr++;
      }
      varname[var_len] = '\0';
      char *var_value = getenv(varname);
      if (var_value) {
        while (*var_value) {
          *res_ptr++ = *var_value++;
        }
      }
    } else {
      *res_ptr++ = *ptr++;
    }
  }
  *res_ptr = '\0';
  return result;
}

// Function to split a line into tokens (arguments)
char **split_line(char *line) {
  int bufsize = 64, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, " \t\r\n\a");
  while (token != NULL) {
    tokens[position] = token;
    position++;

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
  HIST_ENTRY **the_history_list = history_list();
  if (the_history_list) {
    for (int i = 0; the_history_list[i]; i++) {
      printf("%d: %s\n", i + history_base, the_history_list[i]->line);
    }
  }
  return 1;
}

// List of built-in commands
char *builtin_str[] = {
  "cd",
  "exit",
  "export",
  "history"
};

// Corresponding functions for built-in commands
int (*builtin_func[]) (char **) = {
  &shell_cd,
  &shell_exit,
  &shell_export,
  &shell_history
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

  // Check for variable assignment
  char *equals_sign = strchr(args[0], '=');
  if (equals_sign != NULL) {
    *equals_sign = '\0';
    setenv(args[0], equals_sign + 1, 1);
    return 1;
  }

  // Add command to history
  add_history(args[0]);

  int builtin_status = execute_builtin(args);
  if (builtin_status != -1) {
    return builtin_status;
  }

  return launch_program(args);
}

// Function to source .profile
void source_profile() {
  char *home = getenv("HOME");
  if (home) {
    char profile_path[256];
    snprintf(profile_path, sizeof(profile_path), "%s/.profile", home);

    FILE *profile = fopen(profile_path, "r");
    if (profile) {
      char *line = NULL;
      size_t len = 0;
      while (getline(&line, &len, profile) != -1) {
        char *expanded_line = expand_vars(line);
        char **args = split_line(expanded_line);
        execute_command(args);
        free(expanded_line);
        free(args);
      }
      free(line);
      fclose(profile);
    }
  }
}

// Main loop
void shell_loop() {
  char *line;
  char *expanded_line;
  char **args;
  int status;

  rl_bind_keyseq("\\C-p", rl_get_previous_history);
  rl_bind_keyseq("\\C-n", rl_get_next_history);

  do {
    line = readline("$ ");
    if (line == NULL) {
      break;
    }
    expanded_line = expand_vars(line);
    args = split_line(expanded_line);
    status = execute_command(args);

    free(line);
    free(expanded_line);
    free(args);
  } while (status);
}

// Main function
int main(int argc, char **argv) {
  // Source the .profile file if it exists
  //source_profile();

  // Run command loop.
  shell_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}

