/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>.
 * All Rights Reserved.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 *
 *
 * minibox.c is the main program source with all programs as functions called
 * from main().
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

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <linux/limits.h>
#include <linux/stat.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <utmp.h>

/* minibox specific defines */
#define VERSION "0.1.1"

/* Function to compare two directory entries for qsort */
int compare_entries(const void *a, const void *b) {
  return strcmp(*(const char **)a, *(const char **)b);
}

// Comparison function for qsort
int compare_lines(const void *a, const void *b) {
  return strcmp(*(const char **)a, *(const char **)b);
}

// Function to convert signal name to signal number
int signal_name_to_number(const char *name) {
  if (name == NULL)
    return -1;

  // List of known signals
  static const struct {
    const char *name;
    int number;
  } signals[] = {
      {"SIGHUP", SIGHUP},       {"SIGINT", SIGINT},       {"SIGQUIT", SIGQUIT},
      {"SIGILL", SIGILL},       {"SIGTRAP", SIGTRAP},     {"SIGABRT", SIGABRT},
      {"SIGBUS", SIGBUS},       {"SIGFPE", SIGFPE},       {"SIGKILL", SIGKILL},
      {"SIGUSR1", SIGUSR1},     {"SIGSEGV", SIGSEGV},     {"SIGUSR2", SIGUSR2},
      {"SIGPIPE", SIGPIPE},     {"SIGALRM", SIGALRM},     {"SIGTERM", SIGTERM},
      {"SIGSTKFLT", SIGSTKFLT}, {"SIGCHLD", SIGCHLD},     {"SIGCONT", SIGCONT},
      {"SIGSTOP", SIGSTOP},     {"SIGTSTP", SIGTSTP},     {"SIGTTIN", SIGTTIN},
      {"SIGTTOU", SIGTTOU},     {"SIGURG", SIGURG},       {"SIGXCPU", SIGXCPU},
      {"SIGXFSZ", SIGXFSZ},     {"SIGVTALRM", SIGVTALRM}, {"SIGPROF", SIGPROF},
      {"SIGWINCH", SIGWINCH},   {"SIGIO", SIGIO},         {"SIGPWR", SIGPWR},
      {"SIGSYS", SIGSYS}};

  for (size_t i = 0; i < sizeof(signals) / sizeof(signals[0]); i++) {
    if (strcmp(name, signals[i].name) == 0) {
      return signals[i].number;
    }
  }
  return -1; // Signal name not found
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
/* cat [infile] */
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
      if (finptr != stdin)
        fclose(finptr);
      return 1;
    }
  }

  while ((chars = fgetc(finptr)) != EOF) {
    fputc(chars, foutptr);
  }

  if (finptr != stdin)
    fclose(finptr);
  if (foutptr != stdout)
    fclose(foutptr);

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
int _true(void) { return 0; }

/* false program */
/* return false or 1 */
int _false(void) { return 1; }

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
    fprintf(stderr, "ls: access failure - '%s': No such file or directory\n",
            dirpath);
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
        fprintf(stderr, "Memory (re)allocation error\n");
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
/* Basic implementation of init - not production level yet */
int init(void) {
  printf("MiniBox %s init: Running init process\n", VERSION);
  printf("MiniBox %s init: World Initialized -- Hello, World!\n", VERSION);

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
    exit(1);
  }

  while (1) {
    pause(); // Wait for signal
  }

  return 0;
}

/* cmp program */
/* Usage: cmp [comparer] [comparent] */
int cmp(const char *file1, const char *file2) {
  FILE *f1 = fopen(file1, "r");
  FILE *f2 = fopen(file2, "r");
  int ch1, ch2;

  if (!f1 || !f2) {
    fprintf(stderr, "cmp: Cannot open file\n");
    return 2;
  }

  while ((ch1 = fgetc(f1)) != EOF && (ch2 = fgetc(f2)) != EOF) {
    if (ch1 != ch2) {
      printf("Files differ\n");
      fclose(f1);
      fclose(f2);
      return 1;
    }
  }

  if ((ch1 != EOF) || (ch2 != EOF)) {
    printf("Files differ\n");
    fclose(f1);
    fclose(f2);
    return 1;
  }

  fclose(f1);
  fclose(f2);
  return 0;
}

/* rm program */
/* Remove files or directories */
/* Usage: rm [-r] file... */
int rm(int argc, char *argv[]) {
  int recursive = 0;

  // Check for the -r flag
  if (argc > 1 && strcmp(argv[1], "-r") == 0) {
    recursive = 1;
    argc--;
    argv++;
  }

  if (argc < 2) {
    fprintf(stderr, "Usage: rm [-r] file...\n");
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    struct stat st;
    if (stat(argv[i], &st) < 0) {
      fprintf(stderr, "rm: cannot stat '%s': %s\n", argv[i], strerror(errno));
      return 1;
    }

    if (S_ISDIR(st.st_mode)) {
      if (recursive) {
        // Recursively delete directory contents
        DIR *dir = opendir(argv[i]);
        if (dir == NULL) {
          fprintf(stderr, "rm: cannot open directory '%s': %s\n", argv[i],
                  strerror(errno));
          return 1;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
          if (strcmp(entry->d_name, ".") != 0 &&
              strcmp(entry->d_name, "..") != 0) {
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "%s/%s", argv[i], entry->d_name);
            if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
              // Recursively remove subdirectories
              char *args[] = {"rm", "-r", path};
              rm(3, args);
            } else {
              unlink(path);
            }
          }
        }
        closedir(dir);
        rmdir(argv[i]);
      } else {
        fprintf(stderr, "rm: '%s' is a directory\n", argv[i]);
        return 1;
      }
    } else {
      if (unlink(argv[i]) < 0) {
        fprintf(stderr, "rm: cannot remove '%s': %s\n", argv[i],
                strerror(errno));
        return 1;
      }
    }
  }

  return 0;
}

/* rmdir program */
/* Remove empty directories */
/* Usage: rmdir directory... */
int rmdir_cmd(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: rmdir directory...\n");
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    if (rmdir(argv[i]) < 0) {
      fprintf(stderr, "rmdir: failed to remove '%s': %s\n", argv[i],
              strerror(errno));
      return 1;
    }
  }

  return 0;
}

/* mkdir program */
/* Create directories */
/* Usage: mkdir [-p] directory... */
int mkdir_cmd(int argc, char *argv[]) {
  int parents = 0;
  int start_index = 1;

  // Check for the -p flag
  if (argc > 1 && strcmp(argv[1], "-p") == 0) {
    parents = 1;
    start_index = 2;
  }

  if (argc < start_index) {
    fprintf(stderr, "Usage: mkdir [-p] directory...\n");
    return 1;
  }

  for (int i = start_index; i < argc; i++) {
    if (parents) {
      char *path = strdup(argv[i]);
      if (!path) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
      }

      char *p = path;
      while ((p = strchr(p, '/')) != NULL) {
        *p = '\0';
        if (mkdir(path, 0755) < 0 && errno != EEXIST) {
          fprintf(stderr, "mkdir: failed to create directory '%s': %s\n", path,
                  strerror(errno));
          free(path);
          return 1;
        }
        *p = '/';
        p++;
      }
      if (mkdir(argv[i], 0755) < 0 && errno != EEXIST) {
        fprintf(stderr, "mkdir: failed to create directory '%s': %s\n", argv[i],
                strerror(errno));
        free(path);
        return 1;
      }
      free(path);
    } else {
      if (mkdir(argv[i], 0755) < 0 && errno != EEXIST) {
        fprintf(stderr, "mkdir: failed to create directory '%s': %s\n", argv[i],
                strerror(errno));
        return 1;
      }
    }
  }

  return 0;
}

/* mknod program */
/* Usage: mknod name type [major minor] */
int mknod_command(int argc, char *argv[]) {
  if (argc < 4) {
    fprintf(stderr, "Usage: mknod name type [major minor]\n");
    return 1;
  }

  const char *name = argv[1];
  char type = argv[2][0];
  mode_t mode;
  dev_t dev = 0;

  switch (type) {
  case 'p':
    mode = S_IFIFO;
    break;
  case 'c':
  case 'u':
    if (argc < 6) {
      fprintf(stderr, "Usage: mknod name c [major minor]\n");
      return 1;
    }
    mode = S_IFCHR;
    dev = makedev(atoi(argv[3]), atoi(argv[4]));
    break;
  case 'b':
    if (argc < 6) {
      fprintf(stderr, "Usage: mknod name b [major minor]\n");
      return 1;
    }
    mode = S_IFBLK;
    dev = makedev(atoi(argv[3]), atoi(argv[4]));
    break;
  default:
    fprintf(stderr, "Invalid type: %c\n", type);
    return 1;
  }

  if (mknod(name, mode | 0666, dev) == -1) {
    perror("mknod");
    return 1;
  }

  return 0;
}

/* hostname program */
int hostname(void) {
  char hostname[1024];
  if (gethostname(hostname, sizeof(hostname)) == -1) {
    perror("gethostname");
    return 1;
  }
  printf("%s\n", hostname);
  return 0;
}

/* free program */
/* Display the amount of free and used memory in the system */
/* FIXME: free displays 0 for shared while coreutils free displays non-zero */
int free_cmd(void) {
  FILE *file = fopen("/proc/meminfo", "r");
  if (!file) {
    perror("fopen");
    return 1;
  }

  char line[256];
  unsigned long total = 0, free = 0, available = 0, buffers = 0, cached = 0;

  while (fgets(line, sizeof(line), file)) {
    if (sscanf(line, "MemTotal: %lu kB", &total) == 1 ||
        sscanf(line, "MemFree: %lu kB", &free) == 1 ||
        sscanf(line, "MemAvailable: %lu kB", &available) == 1 ||
        sscanf(line, "Buffers: %lu kB", &buffers) == 1 ||
        sscanf(line, "Cached: %lu kB", &cached) == 1) {
      // Parsed a line
    }
  }

  fclose(file);

  unsigned long used = total - free - buffers - cached;

  printf("            total       used       free     shared  buff/cache  "
         "available\n");
  printf("Mem:   %10lu %10lu %10lu %10lu %10lu %10lu\n", total, used, free, 0UL,
         buffers + cached, available);

  return 0;
}

/* xxd program: hex dump */
int xxd(int argc, char *argv[]) {
  FILE *fp;
  unsigned char buffer[16];
  size_t bytesRead;

  if (argc < 2) {
    fprintf(stderr, "Usage: xxd <file>\n");
    return 1;
  }

  fp = fopen(argv[1], "rb");
  if (!fp) {
    perror("fopen");
    return 1;
  }

  while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
    printf("%08zx: ", (unsigned int)ftell(fp) - bytesRead);
    for (size_t i = 0; i < bytesRead; i++) {
      printf("%02x ", buffer[i]);
    }
    for (size_t i = bytesRead; i < sizeof(buffer); i++) {
      printf("   ");
    }
    printf("| ");
    for (size_t i = 0; i < bytesRead; i++) {
      printf("%c", (buffer[i] >= 32 && buffer[i] <= 126) ? buffer[i] : '.');
    }
    printf("\n");
  }

  fclose(fp);
  return 0;
}

/* od program: octal dump */
/* FIXME: First output column should be in octal not hexadecimal */
int od(int argc, char *argv[]) {
  FILE *fp;
  unsigned char buffer[16];
  size_t bytesRead;

  if (argc < 2) {
    fprintf(stderr, "Usage: od <file>\n");
    return 1;
  }

  fp = fopen(argv[1], "rb");
  if (!fp) {
    perror("fopen");
    return 1;
  }

  while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
    printf("%08zx: ", (unsigned int)ftell(fp) - bytesRead);
    for (size_t i = 0; i < bytesRead; i++) {
      printf("%02x ", buffer[i]);
    }
    for (size_t i = bytesRead; i < sizeof(buffer); i++) {
      printf("   ");
    }
    printf("\n");
  }

  fclose(fp);
  return 0;
}

/* hexdump program: display a file in hex */
int hexdump(int argc, char *argv[]) {
  FILE *fp;
  unsigned char buffer[16];
  size_t bytesRead;

  if (argc < 2) {
    fprintf(stderr, "Usage: hexdump <file>\n");
    return 1;
  }

  fp = fopen(argv[1], "rb");
  if (!fp) {
    perror("fopen");
    return 1;
  }

  while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
    for (size_t i = 0; i < bytesRead; i++) {
      printf("%02x ", buffer[i]);
    }
    printf("\n");
  }

  fclose(fp);
  return 0;
}

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

/* vmstat program */
/* report virtual memory statistics */
/* FIXME: Outputs info not in their respective columns */
int vmstat(void) {
  struct sysinfo info;

  if (sysinfo(&info) != 0) {
    perror("sysinfo");
    return 1;
  }

  printf("procs -----------memory---------- ---swap-- -----io---- -system-- "
         "------cpu-----\n");
  printf(" r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us "
         "sy id wa st\n");
  printf(" %2d %2d %6ld %6ld %6ld %6ld %4ld %4ld %5ld %5ld %4d %4d %2d %2d %2d "
         "%2d %2d\n",
         0, 0, info.totalswap - info.freeswap, info.freeram / 1024,
         info.bufferram / 1024, (info.freeram - info.bufferram) / 1024, 0L, 0L,
         0L, 0L, 0, 0, 0, 0, 100, 0, 0);
  return 0;
}

/* cut program */
int cut(int argc, char *argv[]) {
  char delimiter = '\t';  // Default delimiter (tab)
  char *field_str = NULL; // Field string
  int opt;
  size_t num_fields = 0;
  int *fields = NULL;

  // Parse command-line options
  while ((opt = getopt(argc, argv, "d:f:")) != -1) {
    switch (opt) {
    case 'd':
      delimiter = optarg[0];
      break;
    case 'f':
      field_str = optarg;
      break;
    default:
      fprintf(stderr, "Usage: %s [-d delimiter] -f fields [file...]\n",
              argv[0]);
      return EXIT_FAILURE;
    }
  }

  if (!field_str) {
    fprintf(stderr, "Field specification is required.\n");
    return EXIT_FAILURE;
  }

  // Parse the field argument
  size_t fields_capacity = 10;
  fields = malloc(fields_capacity * sizeof(int));
  if (!fields) {
    perror("malloc");
    return EXIT_FAILURE;
  }

  char *str = strdup(field_str);
  if (!str) {
    perror("strdup");
    free(fields);
    return EXIT_FAILURE;
  }

  char *token = strtok(str, ",");
  while (token != NULL) {
    if (num_fields >= fields_capacity) {
      fields_capacity *= 2;
      fields = realloc(fields, fields_capacity * sizeof(int));
      if (!fields) {
        perror("realloc");
        free(str);
        return EXIT_FAILURE;
      }
    }
    fields[num_fields++] = atoi(token);
    token = strtok(NULL, ",");
  }
  free(str);

  // Process lines from stdin or files
  if (optind == argc) {
    // No files specified, read from stdin
    char line[1024];
    while (fgets(line, sizeof(line), stdin)) {
      char *temp_line = strdup(line);
      if (!temp_line) {
        perror("strdup");
        free(fields);
        return EXIT_FAILURE;
      }

      char *token = strtok(temp_line, &delimiter);
      int current_field = 1;
      size_t field_index = 0;

      while (token != NULL) {
        while (field_index < num_fields &&
               fields[field_index] < current_field) {
          field_index++;
        }
        if (field_index < num_fields && fields[field_index] == current_field) {
          printf("%s\n", token);
        }
        current_field++;
        token = strtok(NULL, &delimiter);
      }

      free(temp_line);
    }
  } else {
    // Process each file
    for (int i = optind; i < argc; i++) {
      FILE *file = fopen(argv[i], "r");
      if (!file) {
        perror(argv[i]);
        continue;
      }

      char line[1024];
      while (fgets(line, sizeof(line), file)) {
        char *temp_line = strdup(line);
        if (!temp_line) {
          perror("strdup");
          fclose(file);
          free(fields);
          return EXIT_FAILURE;
        }

        char *token = strtok(temp_line, &delimiter);
        int current_field = 1;
        size_t field_index = 0;

        while (token != NULL) {
          while (field_index < num_fields &&
                 fields[field_index] < current_field) {
            field_index++;
          }
          if (field_index < num_fields &&
              fields[field_index] == current_field) {
            printf("%s\n", token);
          }
          current_field++;
          token = strtok(NULL, &delimiter);
        }

        free(temp_line);
      }

      fclose(file);
    }
  }

  free(fields);
  return EXIT_SUCCESS;
}

/* grep program */
int grep(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <pattern> <file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  const char *pattern = argv[1];
  const char *filename = argv[2];
  FILE *file = fopen(filename, "r");

  if (!file) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  char line[1024]; // Buffer to store each line read from the file
  while (fgets(line, sizeof(line), file)) {
    if (strstr(line, pattern)) { // Check if the pattern is in the current line
      printf("%s", line);        // Print the matching line
    }
  }

  fclose(file);
  return EXIT_SUCCESS;
}

/* tr program */
int tr(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s SET1 SET2\n", argv[0]);
    return EXIT_FAILURE;
  }

  const char *set1 = argv[1];
  const char *set2 = argv[2];
  if (strlen(set1) != strlen(set2)) {
    fprintf(stderr, "SET1 and SET2 must be of the same length.\n");
    return EXIT_FAILURE;
  }

  // Create a translation table
  int translate[256];
  for (int i = 0; i < 256; ++i) {
    translate[i] = i; // Default to no translation
  }

  for (size_t i = 0; i < strlen(set1); ++i) {
    translate[(unsigned char)set1[i]] = (unsigned char)set2[i];
  }

  // Read from standard input (or file if provided)
  int c;
  while ((c = getchar()) != EOF) {
    putchar(translate[c]);
  }

  return EXIT_SUCCESS;
}

/* sort program */
int sort(int argc, char *argv[]) {
  char *lines[1000]; // Array to store lines (simple fixed-size buffer)
  size_t count = 0;
  size_t capacity = sizeof(lines) / sizeof(lines[0]);

  // Read lines from standard input or file if provided
  FILE *file = stdin;
  if (argc == 2) {
    file = fopen(argv[1], "r");
    if (!file) {
      perror("Error opening file");
      return EXIT_FAILURE;
    }
  }

  char buffer[1024];
  while (fgets(buffer, sizeof(buffer), file)) {
    if (count >= capacity) {
      fprintf(stderr, "Too many lines to sort\n");
      return EXIT_FAILURE;
    }
    // Remove newline character and allocate memory for line
    buffer[strcspn(buffer, "\n")] = '\0';
    lines[count] = strdup(buffer);
    if (!lines[count]) {
      perror("Memory allocation failed");
      return EXIT_FAILURE;
    }
    count++;
  }

  if (file != stdin) {
    fclose(file);
  }

  // Sort the lines
  qsort(lines, count, sizeof(char *), compare_lines);

  // Print the sorted lines
  for (size_t i = 0; i < count; i++) {
    puts(lines[i]);
    free(lines[i]);
  }

  return EXIT_SUCCESS;
}

/* uniq program */
int uniq(int argc, char *argv[]) {
  char *prev_line = NULL;
  char buffer[1024];

  // Read lines from standard input or file if provided
  FILE *file = stdin;
  if (argc == 2) {
    file = fopen(argv[1], "r");
    if (!file) {
      perror("Error opening file");
      return EXIT_FAILURE;
    }
  }

  while (fgets(buffer, sizeof(buffer), file)) {
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

    if (prev_line == NULL || strcmp(prev_line, buffer) != 0) {
      if (prev_line != NULL) {
        puts(prev_line);
        free(prev_line);
      }
      prev_line = strdup(buffer);
      if (!prev_line) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
      }
    }
  }

  if (prev_line != NULL) {
    puts(prev_line);
    free(prev_line);
  }

  if (file != stdin) {
    fclose(file);
  }

  return EXIT_SUCCESS;
}

/* uptime program */
int uptime(int argc, char *argv[]) {
  struct sysinfo info;

  if (sysinfo(&info) != 0) {
    perror("sysinfo");
    return EXIT_FAILURE;
  }

  // Calculate uptime in days, hours, minutes, and seconds
  long uptime_seconds = info.uptime;
  long days = uptime_seconds / (24 * 3600);
  uptime_seconds %= (24 * 3600);
  long hours = uptime_seconds / 3600;
  uptime_seconds %= 3600;
  long minutes = uptime_seconds / 60;
  long seconds = uptime_seconds % 60;

  // Print uptime
  printf("uptime: %ld day(s), %ld:%02ld:%02ld\n", days, hours, minutes,
         seconds);

  return EXIT_SUCCESS;
}

/* ps program */
int ps(int argc, char *argv[]) {
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

/* kill program */
int kill_process(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s [-signal] PID...\n", argv[0]);
    return EXIT_FAILURE;
  }

  int signal = SIGTERM; // Default signal

  if (argc >= 3 && argv[1][0] == '-') {
    if (argv[1][1] == 'S') {
      // Named signal (e.g., -SIGHUP)
      signal = signal_name_to_number(argv[1] + 1);
      if (signal == -1) {
        fprintf(stderr, "Unknown signal: %s\n", argv[1]);
        return EXIT_FAILURE;
      }
    } else {
      // Signal number (e.g., -9)
      char *endptr;
      signal = strtol(argv[1] + 1, &endptr, 10);
      if (*endptr != '\0' || signal <= 0 || signal >= NSIG) {
        fprintf(stderr, "Invalid signal number: %s\n", argv[1]);
        return EXIT_FAILURE;
      }
    }
    // Shift arguments
    argv++;
    argc--;
  }

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [-signal] PID...\n", argv[0]);
    return EXIT_FAILURE;
  }

  for (int i = 1; i < argc; i++) {
    char *endptr;
    pid_t pid = strtol(argv[i], &endptr, 10);
    if (*endptr != '\0' || pid <= 0) {
      fprintf(stderr, "Invalid PID: %s\n", argv[i]);
      return EXIT_FAILURE;
    }

    if (kill(pid, signal) == -1) {
      if (errno == ESRCH) {
        fprintf(stderr, "No such process: %d\n", pid);
      } else if (errno == EPERM) {
        fprintf(stderr, "Permission denied to send signal to PID %d\n", pid);
      } else {
        perror("kill");
      }
      return EXIT_FAILURE;
    }

    printf("Sent signal %d to PID %d\n", signal, pid);
  }

  return EXIT_SUCCESS;
}

/* tty program */
int tty(void) {
  char *tty_path;
  int fd;

  // Get the path of the terminal device
  tty_path = ttyname(STDIN_FILENO);
  if (!tty_path) {
    fprintf(stderr, "Not a terminal.\n");
    return 1;
  }

  // Attempt to open the terminal device to ensure it's valid
  fd = open(tty_path, O_RDONLY);
  if (fd < 0) {
    perror("tty");
    return 1;
  }

  // Print the terminal path
  printf("%s\n", tty_path);

  // Close the file descriptor
  close(fd);

  return 0;
}

/* link program */
int create_link(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s FILE1 FILE2\n", argv[0]);
    return 1;
  }

  if (link(argv[1], argv[2]) != 0) {
    perror("link");
    return 1;
  }
  return 0;
}

/* unlink program */
int remove_link(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
    return 1;
  }

  if (unlink(argv[1]) != 0) {
    perror("unlink");
    return 1;
  }
  return 0;
}

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

/* dirname program */
int print_dirname(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <path>\n", argv[0]);
    return 1;
  }

  char *path_copy = strdup(argv[1]);
  if (!path_copy) {
    perror("strdup");
    return 1;
  }

  char *dir = dirname(path_copy);
  printf("%s\n", dir);

  free(path_copy);
  return 0;
}

/* basename program */
int print_basename(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <path>\n", argv[0]);
    return 1;
  }

  char *path_copy = strdup(argv[1]);
  if (!path_copy) {
    perror("strdup");
    return 1;
  }

  char *base = basename(path_copy);
  printf("%s\n", base);

  free(path_copy);
  return 0;
}

/* Update main function */
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
      // FIXME: Integrate this into the wc function
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
    return yes(argc > 1 ? argv : NULL);
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
  } else if (strcmp(cmd, "rm") == 0) {
    return rm(argc, argv);
  } else if (strcmp(cmd, "rmdir") == 0) {
    return rmdir_cmd(argc, argv);
  } else if (strcmp(cmd, "mkdir") == 0) {
    return mkdir_cmd(argc, argv);
  } else if (strcmp(cmd, "mknod") == 0) {
    return mknod_command(argc, argv);
  } else if (strcmp(cmd, "hostname") == 0) {
    return hostname();
  } else if (strcmp(cmd, "free") == 0) {
    return free_cmd();
  } else if (strcmp(cmd, "xxd") == 0) {
    return xxd(argc, argv);
  } else if (strcmp(cmd, "od") == 0) {
    return od(argc, argv);
  } else if (strcmp(cmd, "hexdump") == 0) {
    return hexdump(argc, argv);
  } else if (strcmp(cmd, "w") == 0) {
    return w();
  } else if (strcmp(cmd, "vmstat") == 0) {
    return vmstat();
  } else if (strcmp(cmd, "cut") == 0) {
    return cut(argc, argv);
  } else if (strcmp(cmd, "grep") == 0) {
    return grep(argc, argv);
  } else if (strcmp(cmd, "tr") == 0) {
    return tr(argc, argv);
  } else if (strcmp(cmd, "sort") == 0) {
    return sort(argc, argv);
  } else if (strcmp(cmd, "uniq") == 0) {
    return uniq(argc, argv);
  } else if (strcmp(cmd, "uptime") == 0) {
    return uptime(argc, argv);
  } else if (strcmp(cmd, "ps") == 0) {
    return ps(argc, argv);
  } else if (strcmp(cmd, "kill") == 0) {
    return kill_process(argc, argv);
  } else if (strcmp(cmd, "tty") == 0) {
    return tty();
  } else if (strcmp(cmd, "link") == 0) {
    return create_link(argc, argv);
  } else if (strcmp(cmd, "unlink") == 0) {
    return remove_link(argc, argv);
  } else if (strcmp(cmd, "nohup") == 0) {
    return nohup(argc, argv);
  } else if (strcmp(cmd, "dirname") == 0) {
    return print_dirname(argc, argv);
  } else if (strcmp(cmd, "basename") == 0) {
    return print_basename(argc, argv);
  } else {
    printf("MiniBox %s: A multi-call binary that combines many common Unix "
           "utilities\n"
           "into one that aims to be lightweight and memory efficient.\n"
           "\n"
           "This is free software with ABSOLUTELY NO WARRANTY.\n"
           "For details see the LICENSE that came with this MiniBox "
           "distribution.\n"
           "\n"
           "Current implementations include (in chronological order from 1st "
           "to recently developed):\n"
           "wc:       Print newline, word, and byte counts\n"
           "cat:      Concatenate files\n"
           "cp:       Copy files\n"
           "sync:     Sync filesystem caches to disk\n"
           "yes:      Output y or a character repeatedly until killed\n"
           "update:   Sync filesystem caches every 30 seconds\n"
           "sleep:    Sleep for the specified amount of seconds\n"
           "whoami:   Print current effective username\n"
           "true:     Return true\n"
           "false:    Return false\n"
           "ls:       List directory contents\n"
           "echo:     Display a line of text\n"
           "init:     Initialize system\n"
           "cmp:      Compare two files\n"
           "rm:       Remove files or directories\n"
           "rmdir:    Remove empty directories\n"
           "mkdir:    Create directories\n"
           "mknod:    Create special files\n"
           "hostname: Print hostname\n"
           "free:     Display memory stats (shared & used probably broken)\n"
           "xxd:      Make a hex dump\n"
           "od:       Dump files in octal format\n"
           "hexdump:  Display a file in hexadecimal\n"
           "w:        Display info about current users on machine (output "
           "broken)\n"
           "vmstat:   Report virtual memory statistics (output broken)\n"
           "cut:      Exclude sections of lines in files and print to stdout\n"
           "grep:     Print lines that match the pattern in files\n"
           "tr:       Translate characters\n"
           "sort:     Sort lines of text\n"
           "uniq:     Report or omit duplicate lines\n"
           "uptime:   Display how long the system has been running\n"
           "ps:       Print current running processes snapshot\n"
           "kill:     Send a signal to a process\n"
           "tty:      Print the terminal connected to stdin\n"
           "link:     Call link() to create a link to a file\n"
           "unlink:   Call unlink() to remove a link to a file\n"
           "nohup:    Run command immune to SIGHUP and all output to file\n"
           "dirname:  Strip last component from filename\n"
           "basename: Strip directory and suffix from filenames\n",
           VERSION);
    return 1;
  }
}
