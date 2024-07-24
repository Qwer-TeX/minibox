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
#include <sys/stat.h>

/* minibox specific defines */
#define VERSION "0.1.0"

/* Function to compare two directory entries for qsort */
int compare_entries(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

/* wc program */
/* Usage: wc [<] [infile] */
int wc(FILE *strmin, FILE *strmout) {
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
    return 0;
}

/* common code for the cp and cat program */
/* cat [<] [infile] [|>] outfile */
/* cp [source] [destination] */
int cpcat(const char *strmin, const char *strmout) {
    FILE *finptr;
    FILE *foutptr;
    int chars;

    if (strmin == NULL || strcmp(strmin, "-") == 0) {
        finptr = stdin;
    } else {
        finptr = fopen(strmin, "r");
        if (finptr == NULL) {
            fprintf(stderr, "Error: Cannot open input file %s\n", strmin);
            return 1;
        }
    }

    if (strmout == NULL || strcmp(strmout, "-") == 0) {
        foutptr = stdout;
    } else {
        foutptr = fopen(strmout, "w");
        if (foutptr == NULL) {
            fprintf(stderr, "Error: Cannot open output file %s\n", strmout);
            if (finptr != stdin) fclose(finptr);
            return 1;
        }
    }

    while ((chars = fgetc(finptr)) != EOF) {
        fputc(chars, foutptr);
    }

    if (finptr != stdin) fclose(finptr);
    if (foutptr != stdout) fclose(foutptr);

    return 0;
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
    while (1) {
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
        return 1;
    }

    secs = strtol(argsv[1], &endptr, 10);

    if (errno != 0 || *endptr != '\0' || secs < 0) {
        fprintf(stderr, "Invalid number of seconds: %s\n", argsv[1]);
        return 1;
    }

    sleep((unsigned int)secs);
    return 0;
}

/* whoami program */
/* print current effective user */
/* Usage: whoami */
int whoami(void) {
    struct passwd *pw_ent = getpwuid(geteuid());
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
            for (size_t i = 0; i < count - 1; i++) {
                free(entries[i]);
            }
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

/* echo program */
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
    printf("MiniBox %s init: Mantle(kernel) passes control to init\n", VERSION);
    printf("MiniBox %s init: Running init process\n", VERSION);
    printf("MiniBox %s init: Hello, World!\n", VERSION);
    printf("MiniBox %s init: World Initialized!\n", VERSION);

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
        while (waitpid(-1, NULL, WNOHANG) > 0) {
            // Reap any zombies
        }
    }

    return 0;
}

/* cmp program */
/* Compare two files byte by byte */
int cmp(const char *file1, const char *file2) {
    FILE *f1, *f2;
    int ch1, ch2;
    int pos = 1;
    int line = 1;

    f1 = fopen(file1, "rb");
    if (!f1) {
        fprintf(stderr, "Error: Cannot open file %s\n", file1);
        return 1;
    }

    f2 = fopen(file2, "rb");
    if (!f2) {
        fprintf(stderr, "Error: Cannot open file %s\n", file2);
        fclose(f1);
        return 1;
    }

    while ((ch1 = fgetc(f1)) != EOF && (ch2 = fgetc(f2)) != EOF) {
        if (ch1 != ch2) {
            printf("%s %s differ: byte %d, line %d\n", file1, file2, pos, line);
            fclose(f1);
            fclose(f2);
            return 1;
        }
        if (ch1 == '\n') line++;
        pos++;
    }

    if (fgetc(f1) != EOF || fgetc(f2) != EOF) {
        printf("%s %s differ: EOF mismatch\n", file1, file2);
        fclose(f1);
        fclose(f2);
        return 1;
    }

    fclose(f1);
    fclose(f2);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 1) {
        fprintf(stderr, "No command specified\n");
        return 1;
    }

    char *cmd = basename(argv[0]);

    if (strcmp(cmd, "wc") == 0) {
        if (argc == 1) {
            return wc(stdin, stdout);
        } else {
            FILE *f = fopen(argv[1], "r");
            if (!f) {
                fprintf(stderr, "wc: %s: No such file or directory\n", argv[1]);
                return 1;
            }
            int res = wc(f, stdout);
            fclose(f);
            return res;
        }
    } else if (strcmp(cmd, "cat") == 0) {
        return cpcat(argc > 1 ? argv[1] : NULL, NULL);
    } else if (strcmp(cmd, "cp") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: cp [source] [destination]\n");
            return 1;
        }
        return cpcat(argv[1], argv[2]);
    } else if (strcmp(cmd, "sync") == 0) {
        return _sync();
    } else if (strcmp(cmd, "yes") == 0) {
        return yes(argv);
    } else if (strcmp(cmd, "update") == 0) {
        return update();
    } else if (strcmp(cmd, "sleep") == 0) {
        return _sleep(argc, argv);
    } else if (strcmp(cmd, "whoami") == 0) {
        return whoami();
    } else if (strcmp(cmd, "true") == 0) {
        return _true();
    } else if (strcmp(cmd, "false") == 0) {
        return _false();
    } else if (strcmp(cmd, "ls") == 0) {
        return ls(argc, argv);
    } else if (strcmp(cmd, "echo") == 0) {
        return echo(argc, argv);
    } else if (strcmp(cmd, "init") == 0) {
        return init();
    } else if (strcmp(cmd, "cmp") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: cmp [file1] [file2]\n");
            return 1;
        }
        return cmp(argv[1], argv[2]);
    } else {
        printf("MiniBox %s: A multi-call binary that combines many common Unix utilities\n"
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
               "init:   Init process (dummy version)\n"
               "rm:     Remove files or directories\n"
               "rmdir:  Remove empty directories\n"
               "mv:     Move or rename files and directories\n", VERSION);
        return 1;
    }
}
