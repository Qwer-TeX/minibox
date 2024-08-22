/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>.
 * All Rights Reserved.
 *
 * Licensed under WTFPL, see file LICENSE in this source tree.
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
 * Licensed under WTFPL, see file LICENSE in this source tree.
 */

#include "minibox.h"
#include "utils.h"

int date(int argc, char *argv[]) {
  int opt;
  time_t current_time;
  struct tm *tm_info;
  char *format = NULL;
  char *time_str = NULL;
  char *file = NULL;
  int use_utc = 0;
  int spec_index = -1;

  // Parse options
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "-u") == 0) {
        use_utc = 1;
      } else if (strcmp(argv[i], "-R") == 0) {
        // RFC-2822 date
        time(&current_time);
        print_rfc2822_time(current_time);
        return 0;
      } else if (strcmp(argv[i], "-I") == 0) {
        if (i + 1 < argc) {
          spec_index = i + 1;
        } else {
          print_usage();
          return 1;
        }
      } else if (strcmp(argv[i], "-s") == 0) {
        if (i + 1 < argc) {
          time_str = argv[++i];
        } else {
          print_usage();
          return 1;
        }
      } else if (strcmp(argv[i], "-d") == 0) {
        if (i + 1 < argc) {
          time_str = argv[++i];
        } else {
          print_usage();
          return 1;
        }
      } else if (strcmp(argv[i], "-D") == 0) {
        if (i + 1 < argc) {
          format = argv[++i];
        } else {
          print_usage();
          return 1;
        }
      } else if (strcmp(argv[i], "-r") == 0) {
        if (i + 1 < argc) {
          file = argv[++i];
        } else {
          print_usage();
          return 1;
        }
      } else {
        print_usage();
        return 1;
      }
    } else {
      print_usage();
      return 1;
    }
  }

  // Handle file modification time
  if (file) {
    print_file_modification_time(file);
    return 0;
  }

  // Handle time string and format
  if (time_str) {
    if (format) {
      current_time = parse_time(time_str, format);
    } else {
      current_time = parse_time(time_str, NULL);
    }
    tm_info = localtime(&current_time);
    if (use_utc) {
      tm_info = gmtime(&current_time);
    }
    char buffer[26];
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s\n", buffer);
  } else {
    // Default behavior: print the current date and time
    time(&current_time);
    tm_info = localtime(&current_time);
    if (use_utc) {
      tm_info = gmtime(&current_time);
    }
    char buffer[26];
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s\n", buffer);
  }

  // Handle ISO-8601 specification
  if (spec_index != -1) {
    const char *spec = argv[spec_index];
    if (spec == NULL) {
      spec = "date";
    }
    print_iso8601_time(current_time, spec);
  }

  return 0;
}

static void print_usage() {
  printf("Usage: date [OPTIONS] [+FMT] [[-s] TIME]\n\n");

  printf("Display or set the system date and time.\n\n");

  printf("OPTIONS:\n");
  printf(
      "  -u               Display time in UTC (Coordinated Universal Time).\n");
  printf("  -s TIME          Set the system date and time to TIME. TIME can be "
         "in various formats:\n");
  printf("                     - @seconds_since_1970 (Unix timestamp)\n");
  printf("                     - hh:mm[:ss]\n");
  printf("                     - [YYYY.]MM.DD-hh:mm[:ss]\n");
  printf("                     - YYYY-MM-DD hh:mm[:ss]\n");
  printf("                     - [[[[[YY]YY]MM]DD]hh]mm[.ss]\n");
  printf(
      "                     - MMDDhhmm[[YY]YY][.ss] (American-style format)\n");
  printf("  -d TIME          Display the specified TIME instead of the current "
         "date and time. TIME formats are the same as for -s.\n");
  printf("  -D FMT           Use the given FMT (strptime format) to parse TIME "
         "in -s/-d options.\n");
  printf("  -r FILE          Show the last modification time of FILE.\n");
  printf("  -R               Output the date in RFC-2822 format.\n");
  printf("  -I[SPEC]         Output the date in ISO-8601 format.\n");
  printf("                   SPEC can be:\n");
  printf("                     - date    : YYYY-MM-DD\n");
  printf("                     - hours   : YYYY-MM-DDTHH:00:00\n");
  printf("                     - minutes : YYYY-MM-DDTHH:MM:00\n");
  printf("                     - seconds : YYYY-MM-DDTHH:MM:SS\n");
  printf("                     - ns      : YYYY-MM-DDTHH:MM:SS (nanoseconds "
         "not included)\n");
  printf("\n");

  printf("Examples:\n");
  printf(
      "  date -u               : Display the current date and time in UTC.\n");
  printf("  date -s \"2024-08-20 14:30:00\" : Set the date and time to August "
         "20, 2024, 14:30.\n");
  printf("  date -d \"2024-08-20\" : Display the date August 20, 2024.\n");
  printf("  date -r somefile.txt  : Display the last modification time of "
         "somefile.txt.\n");
  printf("  date -R               : Display the current date and time in "
         "RFC-2822 format.\n");
  printf("  date -Iminutes        : Display the current date and time in "
         "ISO-8601 format with minutes.\n");
}

static void print_rfc2822_time(time_t timestamp) {
  struct tm *tm_info;
  char buffer[30];
  tm_info = localtime(&timestamp);
  strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S %z", tm_info);
  printf("%s\n", buffer);
}

static void print_iso8601_time(time_t timestamp, const char *spec) {
  struct tm *tm_info;
  char buffer[100];
  tm_info = localtime(&timestamp);

  if (strcmp(spec, "date") == 0) {
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", tm_info);
  } else if (strcmp(spec, "hours") == 0) {
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:00:00", tm_info);
  } else if (strcmp(spec, "minutes") == 0) {
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:00", tm_info);
  } else if (strcmp(spec, "seconds") == 0) {
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", tm_info);
  } else if (strcmp(spec, "ns") == 0) {
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", tm_info);
    // Note: For simplicity, nanoseconds are not included in this
    // implementation.
  } else {
    printf("Invalid ISO-8601 specifier\n");
    return;
  }
  printf("%s\n", buffer);
}

static time_t parse_time(const char *time_str, const char *fmt) {
  struct tm tm = {0};
  time_t timestamp;

  if (fmt) {
    strptime(time_str, fmt, &tm);
  } else {
    // Basic parsing of standard formats
    if (time_str[0] == '@') {
      timestamp = (time_t)atoi(time_str + 1);
      return timestamp;
    } else if (strptime(time_str, "%Y-%m-%d %H:%M:%S", &tm) ||
               strptime(time_str, "%Y-%m-%d", &tm) ||
               strptime(time_str, "%H:%M:%S", &tm)) {
      timestamp = mktime(&tm);
      return timestamp;
    }
    fprintf(stderr, "Invalid time format\n");
    exit(1);
  }

  timestamp = mktime(&tm);
  return timestamp;
}

static void print_file_modification_time(const char *filename) {
  struct stat file_info;
  if (stat(filename, &file_info) == 0) {
    char buffer[26];
    struct tm *tm_info = localtime(&file_info.st_mtime);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s\n", buffer);
  } else {
    perror("stat");
  }
}
