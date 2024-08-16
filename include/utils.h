/* MiniBox is a busybox/toybox like replacement aiming to be lightweight,
 * portable, and memory efficient.
 *
 * Copyright (C) 2024 Robert Johnson et al <mitnew842@gmail.com>.
 * All Rights Reserved.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
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

#ifndef UTILS_H
#define UTILS_H
#include "minibox.h"

// Function declarations
// ls
int compare_entries(const void *a, const void *b);
void print_size(off_t size, int human_readable);
void print_long_format(const char *path, const char *filename,
                       int human_readable);

// sort
int compare_lines(const void *a, const void *b);
// kill
int signal_name_to_number(const char *name);
// main
int check_if_minibox(char *argv0);

#endif // !UTILS_H
