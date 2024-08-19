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

// Function to get the first day of the month
int getFirstDayOfMonth(int year, int month) {
  struct tm time = {0};
  time.tm_year = year - 1900;
  time.tm_mon = month - 1;
  time.tm_mday = 1;

  mktime(&time);
  return time.tm_wday;
}

// Function to get the name of the month
const char *getMonthName(int month) {
  static const char *monthNames[] = {
      "January", "February", "March",     "April",   "May",      "June",
      "July",    "August",   "September", "October", "November", "December"};
  return monthNames[month - 1];
}

// Function to print a single month calendar
void printMonthCalendar(int year, int month) {
  static const char *daysOfWeek[] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
  int daysInMonth;
  int firstDay;

  static const int daysInMonthArray[] = {31, 28, 31, 30, 31, 30,
                                         31, 31, 30, 31, 30, 31};

  if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
    daysInMonth = 29;
  } else {
    daysInMonth = daysInMonthArray[month - 1];
  }

  firstDay = getFirstDayOfMonth(year, month);

  // Print the header
  printf("    %s %d\n", getMonthName(month), year);
  for (int i = 0; i < 7; i++) {
    printf("%s ", daysOfWeek[i]);
  }
  printf("\n");

  for (int i = 0; i < firstDay; i++) {
    printf("   ");
  }

  for (int day = 1; day <= daysInMonth; day++) {
    printf("%2d ", day);
    if ((firstDay + day) % 7 == 0) {
      printf("\n");
    }
  }
  printf("\n");
}

// Function to print a full year calendar
void printYearCalendar(int year) {
  for (int month = 1; month <= 12; month++) {
    printMonthCalendar(year, month);
    printf("\n");
  }
}

// Function to handle calendar display based on arguments
int cal(int argc, char *argv[]) {
  int month = 0;
  int year;

  if (argc == 1) {
    // No arguments: print the current month
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    month = tm.tm_mon + 1;    // tm_mon is 0-based
    year = tm.tm_year + 1900; // tm_year is years since 1900
  } else if (argc == 2 || argc == 3) {
    // Arguments provided
    year = atoi(argv[argc - 1]);
    if (argc == 3) {
      month = atoi(argv[1]);
      if (month < 1 || month > 12) {
        fprintf(stderr, "Invalid month\n");
        return 1;
      }
    }
    if (year < 1) {
      fprintf(stderr, "Invalid year\n");
      return 1;
    }
  } else {
    fprintf(stderr, "Usage: %s [month] year\n", argv[0]);
    return 1;
  }

  if (month == 0) {
    printYearCalendar(year);
  } else {
    printMonthCalendar(year, month);
  }

  return 0;
}
