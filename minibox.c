/* MiniBox is a busybox/toybox like replacement aiming to be lightweight, portable, and 
 * memory efficient.
 *
 * Copyleft (C) 2024-2024 Robert Johnson <mitnew842 AT gmail dot com>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 *
 *
 * minibox.c is the main program source with all programs as functions called from main().
 *
 * When adding programs or features, please consider if they can be
 * accomplished in a sane way with standard unix tools. If they're 
 * programs or features you added, please make sure they are read-
 * able and understandable by a novice programmer, if not, add 
 * comments or let me know.
 *
 * I haven't tested but it could compile on windows systems with MSYS/MinGW or Cygwin.
 * MiniBox should be fairly portable for POSIX systems.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <sys/wait.h>
#include <libgen.h>

/* minibox specific defines */
#define VERSION "0.1.0"

/* Function to compare two directory entries for qsort */
int compare_entries(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

/* wc program */
/* Usage: wc [<] [infile] */
int wc(int retval, FILE *strmin, FILE *strmout) {
    int letter, nline, nchar, nword, inword;

    inword = 0;
    nline = nword = nchar = 0;
    while ((letter = getc(strmin)) != EOF) {
        ++nchar;
        if (letter == '\n')
            ++nline;
        if (letter == ' ' || letter == '\n' || letter == '\t')
            inword = 1;
        else if (inword == 0) {
            inword = 1;
            ++nword;
        }
    }
    fprintf(strmout, "%d  %d  %d\n", nline, nword, nchar);
    return retval;
}

/* common code for the cp and cat program */
/* cat [<] [infile] [|>] outfile */
/* cp [some-random-file] [another-random-file] */
int cpcat(int retval, const char *strmin, const char *strmout, int fromcp) {
    FILE *finptr;
    FILE *foutptr;
    int chars;

    finptr = fopen(strmin, "r");
    if (finptr == NULL) {
        fprintf(stderr, "Error: Cannot open input file %s\n", strmin);
        return 1;
    }

    foutptr = fopen(strmout, "w");
    if (foutptr == NULL) {
        fprintf(stderr, "Error: Cannot open output file %s\n", strmout);
        fclose(finptr);
        return 1;
    }

    while ((chars = fgetc(finptr)) != EOF) {
        fputc(chars, foutptr);
    }

    fclose(finptr);
    fclose(foutptr);

    return retval;
}

/* sync program */
/* commit filesystem caches to disk - sync(2) */
int _sync(void) {
    sync();
    return 0;
}

/* yes program */
/* output y or a character repeatedly until killed */
/* yes [repeatent] */
int yes(char *args[]) {
    for (;;) {
        printf("%s\n", args ? args[1] : "y");
    }
    return 0;
}

int update(void) {
    for (int i = 0; i != 3; ++i)
        close(i);
    chdir("/");
    while (1) {
        _sync();
        sleep(30);
    }
}

/* sleep program */
/* Usage: sleep [seconds] */
int _sleep(int argsc, char *argsv[]) {
    char *endptr;
    errno = 0;
    long secs;

    if (argsc != 2) {
        fprintf(stderr, "Usage: sleep [seconds]\n");
        exit(1);
    }

    secs = strtol(argsv[1], &endptr, 10);

    if (errno != 0 || *endptr != '\0' || secs < 0) {
        fprintf(stderr, "Invalid number of seconds: %s\n", argsv[1]);
        exit(1);
    }

    if (secs > (unsigned int)-1) {
        fprintf(stderr, "Number of seconds not within range: %s\n", argsv[1]);
        exit(1);
    }

    sleep((unsigned int)secs);
    return 0;
}

/* whoami program */
/* print current effective user */
/* Usage: whoami */
int whoami(void) {
    struct passwd *pw_ent;
    pw_ent = getpwuid(geteuid());
    if (pw_ent == NULL) {
        fprintf(stderr, "Error: Cannot determine current user\n");
        return 1;
    }
    printf("%s\n", pw_ent->pw_name);
    return 0;
}

/* true program */
/* return true or 0 */
int _true(void) {
    return 0;
}

/* false program */
/* return false or 1 */
int _false(void) {
    return 1;
}

/* ls program */
/* list directories or files */
/* Usage: ls [file] */
int ls(int argc, char *argv[]) {
    DIR *directory;
    struct dirent *entry;
    char **entries = NULL;
    size_t count = 0;
    size_t capacity = 10;

    const char *dirpath = (argc > 1) ? argv[1] : ".";

    directory = opendir(dirpath);
    if (!directory) {
        fprintf(stderr, "ls: access failure - '%s': No such file or directory\n", dirpath);
        return 1;
    }

    entries = malloc(capacity * sizeof(char *));
    if (!entries) {
        fprintf(stderr, "Memory allocation error\n");
        closedir(directory);
        return 1;
    }

    while ((entry = readdir(directory)) != NULL) {
        if (count >= capacity) {
            capacity *= 2;
            char **new_entries = realloc(entries, capacity * sizeof(char *));
            if (!new_entries) {
                fprintf(stderr, "Memory allocation error\n");
                closedir(directory);
                free(entries);
                return 1;
            }
            entries = new_entries;
        }
        entries[count++] = strdup(entry->d_name);
        if (!entries[count - 1]) {
            fprintf(stderr, "Memory allocation error\n");
            closedir(directory);
            free(entries);
            return 1;
        }
    }

    closedir(directory);

    qsort(entries, count, sizeof(char *), compare_entries);

    for (size_t i = 0; i < count; i++) {
        printf("%s\n", entries[i]);
        free(entries[i]);
    }

    free(entries);
    return 0;
}

/* echo program -- the first complete and lightweight implementation in minibox */
int echo(int argc, char *argv[]) {
    int n_flag = 0;
    int e_flag = 0;
    int E_flag = 1; // -E is default
    int arg_start = 1;

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-') {
                for (int j = 1; argv[i][j] != '\0'; j++) {
                    switch (argv[i][j]) {
                        case 'n':
                            n_flag = 1;
                            break;
                        case 'e':
                            e_flag = 1;
                            E_flag = 0;
                            break;
                        case 'E':
                            E_flag = 1;
                            e_flag = 0;
                            break;
                        default:
                            fprintf(stderr, "Usage: echo [-neE] [string]\n");
                            return 1;
                    }
                }
                arg_start++;
            } else {
                break;
            }
        }
    }

    for (int i = arg_start; i < argc; i++) {
        if (e_flag && !E_flag) {
            for (char *p = argv[i]; *p != '\0'; p++) {
                if (*p == '\\') {
                    switch (*(++p)) {
                        case 'n':
                            putchar('\n');
                            break;
                        case 't':
                            putchar('\t');
                            break;
                        case '\\':
                            putchar('\\');
                            break;
                        default:
                            putchar('\\');
                            putchar(*p);
                            break;
                    }
                } else {
                    putchar(*p);
                }
            }
        } else {
            fputs(argv[i], stdout);
        }
        if (i < argc - 1) {
            putchar(' ');
        }
    }

    if (!n_flag) {
        putchar('\n');
    }

    return 0;
}

/* init program */
/* Basic implementation of BusyBox's init */
int init(void) {
    printf("MiniBox %s init: Running init process\n", VERSION);

    // Example: Starting essential services
    pid_t child_pid = fork();
    if (child_pid == 0) {
        // Child process
        execl("/bin/sh", "/bin/sh", "-c", "/sbin/networking", NULL);
        perror("execl");
        exit(1);
    } else if (child_pid > 0) {
        // Parent process
        printf("MiniBox %s init: Started networking service\n", VERSION);
    } else {
        // Error occurred
        perror("fork");
        return 1;
    }

    // Example: Waiting for children
    while (1) {
        int status;
        pid_t terminated_pid = wait(&status);
        if (terminated_pid == -1) {
            perror("wait");
            break;
        }
        if (WIFEXITED(status)) {
            printf("MiniBox %s init: Process %d exited with status %d\n", VERSION, terminated_pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("MiniBox %s init: Process %d terminated by signal %d\n", VERSION, terminated_pid, WTERMSIG(status));
        }
    }

    return 0;
}

/* Main driver function */
int main(int argc, char *argv[]) {
    if (argc < 1) {
        fprintf(stderr, "Usage: %s command [arguments]\n", argv[0]);
        return 1;
    }

    char *command = basename(argv[0]); // Get the command name from symbolic link

    if (strstr(command, "wc")) {
        return wc(0, stdin, stdout);
    } else if (strstr(command, "cat")) {
        return cpcat(0, stdin, stdout, 0);
    } else if (strstr(command, "cp")) {
        if (argc < 3) {
            fprintf(stderr, "Usage: %s [source] [destination]\n", command);
            return 1;
        }
        return cpcat(0, argv[1], argv[2], 1);
    } else if (strstr(command, "sync")) {
        return _sync();
    } else if (strstr(command, "yes")) {
        return yes(&argv[1]);
    } else if (strstr(command, "update")) {
        return update();
    } else if (strstr(command, "sleep")) {
        return _sleep(argc, argv);
    } else if (strstr(command, "whoami")) {
        return whoami();
    } else if (strstr(command, "true")) {
        return _true();
    } else if (strstr(command, "false")) {
        return _false();
    } else if (strstr(command, "ls")) {
        return ls(argc, argv);
    } else if (strstr(command, "echo")) {
        return echo(argc, argv);
    } else if (strstr(command, "init")) {
        return init();
    } else {
        printf("MiniBox %s: A multi-call binary that combines many common unix utilities\n"
               "into one that aims to be lightweight and memory efficient.\n"
               "\n"
               "This is free software with ABSOLUTELY NO WARRANTY.\n"
               "For details see the LICENSE that came with this distribution.\n"
               "\n"
               "Current implementations include (in chronological order from 1st to last developed):\n"
               "wc:     Print newline, word, and byte counts\n"
               "cat:    Concatenate files\n"
               "cp:     Copy files\n"
               "sync:   Sync filesystem caches to disk\n"
               "yes:    Output y or a character repeatedly until killed\n"
               "update: Sync filesystem caches every 30 seconds\n"
               "sleep:  Sleep for the specified amount of seconds\n"
               "whoami: Print current effective username\n"
               "true:   return true or 0\n"
               "false:  return false or 1\n"
               "ls:     List files and directories\n"
               "init:   Init process (dummy version)\n", VERSION);
        return 1;
    }

    return 0;
}

