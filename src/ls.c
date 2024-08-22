/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>.
 * All Rights Reserved.
 *
 * Licensed under Unlicense License, see file LICENSE in this source tree.
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
 * Licensed under Unlicense License, see file LICENSE in this source tree.
 */
#include "minibox.h"
#include "utils.h"

int compare_entries(const void *a, const void *b) {
  return strcmp(*(const char **)a, *(const char **)b);
}

void print_size(off_t size, int human_readable) {
  if (!human_readable) {
    printf("%5ld ", (long)size);
    return;
  }

  const char suffixes[] = "BKMGT";
  int i = 0;
  double size_d = size;

  while (size_d >= 1024 && i < sizeof(suffixes) - 1) {
    size_d /= 1024;
    i++;
  }
  printf("%5.1f%c ", size_d, suffixes[i]);
}

void print_long_format(const char *path, const char *filename,
                       int human_readable) {
  struct stat file_info;
  char full_path[1024];

  snprintf(full_path, sizeof(full_path), "%s/%s", path, filename);
  if (stat(full_path, &file_info) != 0) {
    perror("stat");
    return;
  }

  printf("%c", S_ISDIR(file_info.st_mode) ? 'd' : '-');
  printf("%c%c%c%c%c%c%c%c ", (file_info.st_mode & S_IRUSR) ? 'r' : '-',
         (file_info.st_mode & S_IWUSR) ? 'w' : '-',
         (file_info.st_mode & S_IXUSR) ? 'x' : '-',
         (file_info.st_mode & S_IRGRP) ? 'r' : '-',
         (file_info.st_mode & S_IWGRP) ? 'w' : '-',
         (file_info.st_mode & S_IXGRP) ? 'x' : '-',
         (file_info.st_mode & S_IROTH) ? 'r' : '-',
         (file_info.st_mode & S_IWOTH) ? 'w' : '-',
         (file_info.st_mode & S_IXOTH) ? 'x' : '-');
  printf("%lu ", (unsigned long)file_info.st_nlink);

  struct passwd *pw = getpwuid(file_info.st_uid);
  struct group *gr = getgrgid(file_info.st_gid);
  printf("%s %s ", pw ? pw->pw_name : "unknown", gr ? gr->gr_name : "unknown");

  print_size(file_info.st_size, human_readable);

  char time_buf[64];
  strftime(time_buf, sizeof(time_buf), "%b %d %H:%M",
           localtime(&file_info.st_mtime));
  printf("%s %s\n", time_buf, filename);
}

int ls(int argc, char *argv[]) {
  DIR *directory;
  struct dirent *entry;
  char **entries = NULL;
  size_t count = 0, capacity = 10;
  int long_format = 0, human_readable = 0, show_all = 0;
  const char *dirpath = ".";

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      for (int j = 1; argv[i][j]; j++) {
        switch (argv[i][j]) {
        case 'l':
          long_format = 1;
          break;
        case 'h':
          human_readable = 1;
          break;
        case 'a':
          show_all = 1;
          break;
        default:
          fprintf(stderr, "Usage: %s [-lha] [directory]\n", argv[0]);
          return 1;
        }
      }
    } else {
      dirpath = argv[i];
    }
  }

  if (!(directory = opendir(dirpath))) {
    fprintf(stderr, "ls: cannot access '%s': No such file or directory\n",
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
    if (!show_all && entry->d_name[0] == '.')
      continue;

    if (count >= capacity) {
      capacity *= 2;
      entries = realloc(entries, capacity * sizeof(char *));
      if (!entries) {
        fprintf(stderr, "Memory (re)allocation error\n");
        closedir(directory);
        return 1;
      }
    }

    entries[count++] = strdup(entry->d_name);
    if (!entries[count - 1]) {
      fprintf(stderr, "Memory allocation error\n");
      closedir(directory);
      for (size_t k = 0; k < count - 1; k++)
        free(entries[k]);
      free(entries);
      return 1;
    }
  }

  closedir(directory);

  qsort(entries, count, sizeof(char *), compare_entries);

  if (long_format) {
    for (size_t i = 0; i < count; i++) {
      print_long_format(dirpath, entries[i], human_readable);
      free(entries[i]);
    }
  } else {
    for (size_t i = 0; i < count; i++) {
      printf("%s\n", entries[i]);
      free(entries[i]);
    }
  }

  free(entries);
  return 0;
}
