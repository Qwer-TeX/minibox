#ifndef UTMP_WRAPPER_H
#define UTMP_WRAPPER_H
#if defined(__linux__)
#include <utmp.h>
#else
#include <utmpx.h>

/* Ported directly from utmpx.h */
struct utmp {
  short ut_type;        /* Type of entry. */
  struct timeval ut_tv; /* Time entry was made. */
  char ut_id[8];        /* Record identifier. */
  pid_t ut_pid;         /* Process ID. */
  char ut_user[32];     /* User login name. */
  char ut_line[16];     /* Device name. */
  char ut_host[128];    /* Remote hostname. */
};

inline void setutent() { setutxent(); }

inline void endutent() { endutxent(); }

inline struct utmpx *getutent() { return getutxent(); }

#endif
#endif
