/* These two signals are missing from the BSDs; define them here with generic equivalents */
#ifndef SIGSTKFLT
#define SIGSTKFLT SIGFPE
#endif

#ifndef SIGPWR
#define SIGPWR SIGQUIT
#endif