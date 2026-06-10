#include "config.h"
#ifdef CONFIG_PRINTF
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

int printf_cmd(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: printf format [argument...]\n");
    return 1;
  }

  const char *fmt = argv[1];
  int argi = 2;

  while (*fmt) {
    if (*fmt == '%') {
      fmt++;
      int ljust = 0, zero = 0, width = 0;
      while (*fmt == '-' || *fmt == '0') { if (*fmt == '-') ljust=1; else zero=1; fmt++; }
      while (*fmt >= '0' && *fmt <= '9') width = width*10 + (*fmt++ - '0');
      (void)ljust; (void)zero; (void)width; /* not yet implemented in this minimal printf */
      switch (*fmt) {
      case 's': {
        const char *s = (argi < argc) ? argv[argi++] : "";
        fputs(s, stdout);
        break;
      }
      case 'd': case 'i': {
        long v = (argi < argc) ? atol(argv[argi++]) : 0;
        printf("%ld", v);
        break;
      }
      case 'u': {
        unsigned long v = (argi < argc) ? strtoul(argv[argi++], NULL, 10) : 0;
        printf("%lu", v);
        break;
      }
      case 'x': case 'X': {
        unsigned long v = (argi < argc) ? strtoul(argv[argi++], NULL, 16) : 0;
        printf((*fmt == 'x') ? "%lx" : "%lX", v);
        break;
      }
      case 'o': {
        unsigned long v = (argi < argc) ? strtoul(argv[argi++], NULL, 8) : 0;
        printf("%lo", v);
        break;
      }
      case 'c': {
        char c = (argi < argc && argv[argi][0]) ? argv[argi++][0] : 0;
        putchar(c);
        break;
      }
      case '%':
        putchar('%');
        break;
      default:
        putchar('%');
        putchar(*fmt);
        break;
      }
      fmt++;
    } else if (*fmt == '\\') {
      fmt++;
      switch (*fmt) {
      case 'a': putchar('\a'); break;
      case 'b': putchar('\b'); break;
      case 'f': putchar('\f'); break;
      case 'n': putchar('\n'); break;
      case 'r': putchar('\r'); break;
      case 't': putchar('\t'); break;
      case 'v': putchar('\v'); break;
      case '\\': putchar('\\'); break;
      case '0': {
        int v = 0; fmt++;
        for (int k=0; k<3 && *fmt>='0'&&*fmt<='7'; k++) v = v*8 + (*fmt++ - '0');
        putchar(v); fmt--; break;
      }
      default: putchar(*fmt); break;
      }
      fmt++;
    } else {
      putchar(*fmt++);
    }
  }
  return 0;
}
#endif /* CONFIG_PRINTF */
