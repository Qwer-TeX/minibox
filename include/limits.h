#if defined(__linux__)
#include <linux/limits.h>
#else
#include <sys/limits.h>
#define PATH_MAX 4096
#endif
