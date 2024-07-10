#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <errno.h>

/* minibox specific defines */
#define VERSION "0.1.0pre6-testing"

/* Not needed, though put in case */
extern FILE *stdin;
extern FILE *stdout;

/* minibox driver */
int
main ( int argc, char *argv[] )
{

  if (strstr(*argv, "wc"))
    return wc( 0, stdin, stdout );
  else if (strstr(*argv, "cat"))
    return cpcat( 0, stdin, stdout, 0 );
  else if (strstr(*argv, "cp"))
    return cpcat( 0, *++argv, *(argv+2), 1 );
  else if (strstr(*argv, "sync"))
    return _sync();
  else if (strstr(*argv, "yes"))
    return yes(argv[1]);
  else if (strstr(*argv, "update"))
    return update();
  else if (strstr(*argv, "sleep"))
    return _sleep(argc, argv);
  else if (strstr(*argv, "whoami"))
    return whoami();
  else if (strstr(*argv, "true"))
    return _true();
  else if (strstr(*argv, "false"))
    return _false();
  else
    printf("MiniBox %s: A multi-call binary that combines many common unix utilities\n"
           "into one that aims to be lightweight and memory efficient.\n"
           "\n"
           "This is free software with ABSOLUTELY NO WARRANTY.\n"
           "For details see the LICENSE than came with this distribution.\n"
           "\n"
           "Current implementations include (in chronological order from 1st to last developed):\n"
           "wc:     Print newline, word, and byte counts\n"
           "cat:    Concatenate files\n"
           "cp:     Copy files\n"
           "sync:   Sync filesystem caches to disk\n"
           "yes:    Output y or a character repeatedly until killed\n"
           "update: Sync filesystem caches every 30 seconds\n"
           "sleep:  Sleep for the specified amount of seconds\n"
           "whoami: Print current effective username\n"
           "true:   return true or 0\n"
           "false:  return false or 1\n", VERSION);
    return 0;
}

/* wc program */
/* Usage: wc [<] [infile] */
int
wc(int retval, FILE *strmin, FILE *strmout)
{
  int letter, nline, nchar, nword, inword;

  inword = 0;
  nline = nword = nchar = 0;
  while ((letter = getc(strmin)) != EOF)
  {
    ++nchar;
    if (letter == '\n')
      ++nline;
    if (letter == ' ' || letter == '\n' || letter == '\t')
      inword = 1;
    else if (inword == 0) 
    {
      inword == 1;
      ++nword;
    }
  }
  fprintf(stdout, "%d  %d  %d\n", nline, nword, nchar);
  return retval;
}

/* cat and cp program */
/* cat [<] [infile] [|>] outfile */
/* cp [some-random-file] [another-random-file] */
int
cpcat(int retval, FILE *strmin, FILE *strmout, int fromcp)
{
  FILE *finptr;
  FILE *foutptr;
  int chars;

  if (( finptr = fopen( strmin, "r")) != NULL )
  {
    if (( foutptr = fopen( strmout, "w")) != NULL )
    {
      while (( chars = fgetc( finptr )) != EOF)
      {
        fputc( chars, foutptr );
      }
    } 
    else 
    {
      if (fromcp == "1")
        fprintf(stderr, "Contents could not be read to stdout\n");
      else
        fprintf(stderr, "Contents could not be read to %s\n", strmout);
      return 1;
    }
  }
  else
  {
    if (fromcp == "1")
      fprintf(stderr, "Contents could not be read from stdin\n");
    else
      fprintf(stderr, "Contents could not be read from %s\n", strmin);
    return 1;
  }
  return retval;
}

/* sync program */
/* commit filesystem caches to disk - sync(2) */
int
_sync(void)
{
  sync();
  /* This function is always successful - listed as an error in sync(2) */
  return 0;
}

/* yes program */
/* output y or a repeatent repeatedly until killed */
/* yes [repeatent] */
int
yes(char *args[])
{
  for(;;)
    printf("%s\n", args > 1 ? *++args : "y");
  return 0;
}

int
update(void)
{
  /* close all standard descriptors */
  for(int i=0; i!=3; ++i)
    close(i);

  /* change directory to root to avoid locking the device */
  chdir("/");

  while(1!=0)
  {
    _sync();
    sleep(30);
  }
}

/* sleep program */
/* Usage: sleep [seconds] */
int
_sleep(int argsc, char *argsv[]) 
{
  char *endptr;
  errno = 0;
  long secs;

  /* Check if the correct number of arguments is provided */
  if (argsc != 2)
  {
    fprintf(stderr, "Usage: sleep [seconds]\n");
    exit(1);
  }

  /* Validate that the argument is a number */
  secs = strtol(*++argsv, &endptr, 10);

  /* Check for conversion errors just in case */
  if (errno != 0 || *endptr != '\0' || secs < 0) {
      fprintf(stderr, "Invalid number of seconds: %s\n", *++argsv);
      exit(1);
  }

  /* Check if the # of secs is within the valid range for unsigned ints */
  if (secs > (unsigned int)-1) {
      fprintf(stderr, "Number of seconds not within range: %s\n", *++argsv);
      exit(1);
  }

  /* Call sleep with the valid number of seconds */
  sleep((unsigned int)secs);
  return 0;
}

/* whoami program */
/* print current effective user */
/* Usage: whoami */
int
whoami(void)
{
  struct passwd *pw_ent;

  pw_ent = getpwuid(geteuid());
  if (pw_ent == NULL)
    exit(1);
  printf("%s\n", pw_ent->pw_name);
  return 0;
}

/* true program */
/* return true or 0 */
int
_true(void)
{
  exit(0);
}

/* false program */
/* return false or 1 */
int
_false(void)
{
  exit(1);
}
