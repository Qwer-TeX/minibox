#ifndef STAT_WRAPPER_H
#define STAT_WRAPPER_H
#if defined(__linux__)
#include <linux/stat.h>
#else
#include <sys/stat.h>
#endif
#endif
