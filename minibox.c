#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>

/* minibox specific defines */
#define VERSION "0.1.0pre6-testing"

/* Not needed, though put in case */
extern FILE *stdin;
extern FILE *stdout;

/* minibox driver */
int
main ( int argc, char *argv[] )
{

  if (strstr(argv[0], "wc"))
    return wc( 0, stdin, stdout );
  else if (strstr(argv[0], "cat"))
    return cpcat( 0, stdin, stdout, 0 );
  else if (strstr(argv[0], "cp"))
    return cpcat( 0, argv[1], argv[2], 1 );
  else if (strstr(argv[0], "sync"))
    return _sync();
  else if (strstr(argv[0], "yes"))
    return yes(argv[1]);
  else if (strstr(argv[0], "update"))
    return update();
  else if (strstr(argv[0], "sleep"))
    return _sleep(argc, argv);
  else if (strstr(argv[0], "whoami"))
    return whoami();
  else if (strstr(argv[0], "true"))
    return _true();
  else if (strstr(argv[0], "false"))
    return _false();
  else if (strstr(argv[0], "ls"))
    return ls(argc, argv);
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
           "false:  return false or 1\n"
           "ls:     List files and directories\n", VERSION);
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
    printf("%s\n", args > 1 ? args[1] : "y");
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
  secs = strtol(argsv[1], &endptr, 10);

  /* Check for conversion errors just in case */
  if (errno != 0 || *endptr != '\0' || secs < 0) {
      fprintf(stderr, "Invalid number of seconds: %s\n", argsv[1]);
      exit(1);
  }

  /* Check if the # of secs is within the valid range for unsigned ints */
  if (secs > (unsigned int)-1) {
      fprintf(stderr, "Number of seconds not within range: %s\n", argsv[1]);
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

/* ls program */
/* list directories or files */
/* Usage: ls [file] */
int
ls(int argc, char *argv[])
{
  DIR *directory;
  struct dirent *entry;

  if (argc > 1)
    directory = opendir(argv[1]);
  else
    directory = opendir(".");
  if(!directory)
  {
    fprintf(stdout, "ls: access failure - '%s': No such file or directory\n", argv[1]);
    return 1;
  }

  while ((entry = readdir(directory)) != NULL)
  {
    printf("%s\n", entry->d_name);
  }

  closedir(directory);
}

/* Sketch of a echo function with arguments */
//int
//echo(int argc, char *argv[])
//{
//  int n_flag = 0;
//  int e_flag = 0;
//  int arg_start = 1;
//
//  /* Parse options */
//  if (argc > 1) {
//    for (int i = 1; i < argc; i++) {
//      if (strcmp(argv[i], "-n") == 0) {
//        n_flag = 1;
//        arg_start++;
//      } else if (strcmp(argv[i], "-e") == 0) {
//        e_flag = 1;
//        arg_start++;
//      } else if (argv[i][0] == '-') {
//        printf("Usage: echo [-en] [string]\n");
//        return;
//      } else {
//        break;
//      }
//    }
//  }
//
//  /* Print arguments */
//  for (int i = arg_start; i < argc; i++) {
//    if (e_flag) {
//      for (char *p = argv[i]; *p != '\0'; p++) {
//        if (*p == '\\') {
//          switch (*(++p)) {
//            case 'n':
//              putchar('\n');
//              break;
//            case 't':
//              putchar('\t');
//              break;
//            case '\\':
//              putchar('\\');
//              break;
//            default:
//              putchar('\\');
//              putchar(*p);
//              break;
//          }
//        } else {
//          putchar(*p);
//        }
//      }
//    } else {
//      fputs(argv[i], stdout);
//    }
//    if (i < argc - 1) {
//      putchar(' ');
//    }
//  }
//
//  // Print newline if -n is not set
//  if (!n_flag) {
//    putchar('\n');
//  }
//}
//
