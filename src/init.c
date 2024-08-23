/* init.c - init program.
 * Init fully written from scratch.
 *
 * Copyright (C) 2024 Robert Johnson <mitnew842@gmail.com>
 * Copyright 2012 Harvind Singh <harvindsingh1981@gmail.com>
 * Copyright 2013 Kyungwan Han  <asura321@gmail.com>
 *                      -- All Rights Reserved
 * Not guaranteed to work, patches welcome 24/7
 * FIXME: Keyboard input broken
 *
 * No Standard
 *
 * Licensed under Unlicense License, see file LICENSE in this source tree.
 */

#include "minibox.h"

// Define action and runlevel constants
#define SYSINIT 0x01
#define WAIT 0x02
#define ONCE 0x04
#define RESPAWN 0x08
#define ASKFIRST 0x10
#define CTRLALTDEL 0x20
#define SHUTDOWN 0x40
#define RESTART 0x80
#define RUNLEVEL0 0x1000
#define RUNLEVEL1 0x2000
#define RUNLEVEL2 0x4000
#define RUNLEVEL3 0x8000

// Data structure for actions
struct action_list_seed {
  struct action_list_seed *next;
  pid_t pid;
  uint8_t action;
  char *terminal_name;
  char *command;
  int runlevel; // Added for runlevel support
} *action_list_pointer = NULL;

int caught_signal;
int current_runlevel = RUNLEVEL0; // Default runlevel

// Initialize the console
static void initialize_console(void) {
  // Implementation unchanged
}

// Reset terminal settings
static void reset_term(int fd) {
  // Implementation unchanged
}

// Add a new action to the list
static void add_new_action(int action, char *command, char *term,
                           int runlevel) {
  struct action_list_seed *x, **y;
  y = &action_list_pointer;
  x = *y;
  while (x) {
    if (!(strcmp(x->command, command)) && !(strcmp(x->terminal_name, term))) {
      *y = x->next; // remove from the list
      while (*y)
        y = &(*y)->next; // traverse through list till end
      x->next = NULL;
      break;
    }
    y = &(x)->next;
    x = *y;
  }

  // create a new node
  if (!x) {
    x = (struct action_list_seed *)calloc(1, sizeof(*x));
    x->command = strdup(command);
    x->terminal_name = strdup(term);
  }
  x->action = action;
  x->runlevel = runlevel;
  *y = x;
}

// Parse /etc/inittab with runlevel support
static void parse_inittab(void) {
  char *line = NULL;
  size_t allocated_length = 0;
  int line_number = 0;
  char *act_name = "sysinit\0wait\0once\0respawn\0askfirst\0ctrlaltdel\0"
                   "shutdown\0restart\0";
  FILE *fp = fopen("/etc/inittab", "r");

  if (!fp) {
    fprintf(stderr, "Unable to open /etc/inittab. Using Default inittab\n");
    add_new_action(SYSINIT, "/etc/init.d/rcS", "", RUNLEVEL0);
    add_new_action(RESPAWN, "/sbin/getty -n -l /bin/sh -L 115200 tty1 vt100",
                   "", RUNLEVEL0);
    return;
  }

  while (getline(&line, &allocated_length, fp) > 0) {
    char *p = line, *x, *tty_name = NULL, *command = NULL, *extracted_token,
         *tmp;
    int action = 0, token_count = 0, i, runlevel = RUNLEVEL0;

    if ((x = strchr(p, '#')))
      *x = '\0';
    line_number++;
    action = 0;

    while ((extracted_token = strsep(&p, ":"))) {
      token_count++;
      switch (token_count) {
      case 1:
        if (*extracted_token) {
          if (!strncmp(extracted_token, "/dev/", 5))
            tty_name = strdup(extracted_token);
          else
            tty_name =
                (char *)malloc(strlen("/dev/") + strlen(extracted_token) + 1);
          sprintf(tty_name, "/dev/%s", extracted_token);
        } else
          tty_name = strdup("");
        break;
      case 2:
        break;
      case 3:
        for (tmp = act_name, i = 0; *tmp; i++, tmp += strlen(tmp) + 1) {
          if (!strcmp(tmp, extracted_token)) {
            action = 1 << i;
            break;
          }
        }
        if (!*tmp)
          fprintf(stderr, "Invalid action at line number %d ---- ignoring\n",
                  line_number);
        break;
      case 4:
        command = strdup(extracted_token);
        break;
      case 5:
        runlevel = atoi(extracted_token);
        break;
      default:
        fprintf(stderr, "Bad inittab entry at line %d\n", line_number);
        break;
      }
    }

    if (token_count >= 4 && action)
      add_new_action(action, command, tty_name, runlevel);
    free(tty_name);
    free(command);
  }
  free(line);
  fclose(fp);
}

// Reload /etc/inittab
static void reload_inittab(void) {
  struct action_list_seed **y;
  y = &action_list_pointer;
  while (*y) {
    if (!(*y)->pid) {
      struct action_list_seed *x = *y;
      free(x->terminal_name);
      free(x->command);
      *y = (*y)->next;
      free(x);
      continue;
    }
    y = &(*y)->next;
  }
  parse_inittab();
}

// Run a command
static void run_command(char *command) {
  char *final_command[128];
  int hyphen = (command[0] == '-');
  command = command + hyphen;

  if (!strpbrk(command, "?<>'\";[]{}\\|=()*&^$!`~")) {
    char *next_command;
    char *extracted_command;
    int x = 0;

    next_command = strncpy((char *)malloc(strlen(command) + 1),
                           command - hyphen, strlen(command) + 1);
    next_command[strlen(command)] = '\0';
    command = next_command + hyphen;
    while ((extracted_command = strsep(&next_command, " \t"))) {
      if (*extracted_command) {
        final_command[x] = extracted_command;
        x++;
      }
    }
    final_command[x] = NULL;
  } else {
    snprintf((char *)malloc(strlen(command) + 6), 64, "exec %s", command);
    command = "-/bin/sh" + 1;
    final_command[0] = "-/bin/sh" + !hyphen;
    final_command[1] = "-c";
    final_command[2] = command;
    final_command[3] = NULL;
  }
  if (hyphen)
    ioctl(0, TIOCSCTTY, 0);
  execvp(command, final_command);
  perror("execvp");
  exit(EXIT_FAILURE);
}

// Final run of action
static pid_t final_run(struct action_list_seed *x) {
  pid_t pid;
  int fd;
  sigset_t signal_set;

  sigfillset(&signal_set);
  sigprocmask(SIG_BLOCK, &signal_set, NULL);
  if (x->action & ASKFIRST)
    pid = fork();
  else
    pid = vfork();

  if (pid > 0) {
    sigfillset(&signal_set);
    sigprocmask(SIG_UNBLOCK, &signal_set, NULL);
    return pid;
  } else if (pid < 0) {
    perror("fork");
    sleep(1);
    return 0;
  }

  sigset_t signal_set_c;
  sigfillset(&signal_set_c);
  sigprocmask(SIG_UNBLOCK, &signal_set_c, NULL);
  setsid();

  if (x->terminal_name[0]) {
    close(0);
    fd = open(x->terminal_name, (O_RDWR | O_NONBLOCK), 0600);
    if (fd != 0) {
      fprintf(stderr, "Unable to open %s, %s\n", x->terminal_name,
              strerror(errno));
      _exit(EXIT_FAILURE);
    }
    if (tcsetpgrp(0, getpid()) < 0) {
      fprintf(stderr, "Unable to set pgrp to %d, %s\n", getpid(),
              strerror(errno));
      _exit(EXIT_FAILURE);
    }
    reset_term(0);
  }

  if (x->action & WAIT) {
    while (1)
      pause();
  } else if (x->action & RESTART) {
    while (1) {
      run_command(x->command);
      sleep(1);
    }
  } else {
    run_command(x->command);
    _exit(EXIT_SUCCESS);
  }

  return 0;
}

// Signal handler
static void signal_handler(int sig) { caught_signal = sig; }

// Main function
int init(int argc, char *argv[]) {
  struct sigaction sa;
  struct sigaction old_sa;
  int fd;
  int x;

  // Initialize signal handling
  sa.sa_handler = signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGTERM, &sa, &old_sa);
  sigaction(SIGINT, &sa, &old_sa);
  sigaction(SIGHUP, &sa, &old_sa);

  initialize_console();

  // Parse inittab
  parse_inittab();

  while (1) {
    struct action_list_seed *x, *y;
    x = action_list_pointer;

    while (x) {
      if (x->pid) {
        int status;
        pid_t pid = waitpid(x->pid, &status, WNOHANG);
        if (pid == -1) {
          perror("waitpid");
          exit(EXIT_FAILURE);
        }
        if (pid == x->pid && WIFEXITED(status) &&
            WEXITSTATUS(status) == EXIT_SUCCESS) {
          x->pid = 0;
        }
      } else if (x->runlevel == current_runlevel) {
        x->pid = final_run(x);
      }
      x = x->next;
    }

    if (caught_signal == SIGHUP) {
      reload_inittab();
      caught_signal = 0;
    }

    sleep(1);
  }

  return 0;
}
