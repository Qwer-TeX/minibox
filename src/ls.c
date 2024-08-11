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

#include "minibox.h"
#include "utils.h"

/* Function to compare two directory entries for qsort */
int compare_entries(const void *a, const void *b) {
  return strcmp(*(const char **)a, *(const char **)b);
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
