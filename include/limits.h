#ifndef LIMITS_WRAPPER_H
#define LIMITS_WRAPPER_H

#if defined(__linux__)
#include <linux/limits.h>
#elif defined(__APPLE__)
#include <sys/syslimits.h>
#else
#include <limits.h>
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
#endif
#endif
