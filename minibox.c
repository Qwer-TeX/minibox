#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* wc specific defines */
#define WC_IN  1  /* in a word */
#define WC_OUT 0  /* not in a word (default) */

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
    return cat( 0, stdin, stdout, 0 );
  else if (strstr(argv[0], "cp"))
    return cat( 0, argv[1], argv[2], 1 );
  else if (strstr(argv[0], "sync"))
    return _sync();
  else if (strstr(argv[0], "yes"))
    return yes(argv[1]);
  else if (strstr(argv[0], "update"))
    return update();
  else
    printf("MiniBox: A multi-call binary that aims to be lightweight\n"
           "memory efficient\n"
           "\n"
           "Current implementations include (in chronological order from 1st to last developed:\n"
           "wc:     Print newline, word, and byte counts\n"
           "cat:    Concatenate files\n"
           "cp:     Copy files\n"
           "sync:   Sync filesystem caches to disk\n"
           "yes:    Output y or a character repeatedly until killed\n"
           "update: sync filesystem caches every 30 seconds\n");
    return 0;
}

/* wc program */
/* Usage: wc [<] [infile] [|>] [outfile] */
int
wc(int retval, FILE *strmin, FILE *strmout)
{
  int letter, nline, nchar, nword, inword;

  inword = WC_OUT;
  nline = nword = nchar = 0;
  while ((letter = getc(strmin)) != EOF)
  {
    ++nchar;
    if (letter == '\n')
      ++nline;
    if (letter == ' ' || letter == '\n' || letter == '\t')
      inword = WC_IN;
    else if (inword == WC_OUT) 
    {
      inword == WC_IN;
      ++nword;
    }
  }
  fprintf(stdout, "%d  %d  %d\n", nline, nword, nchar);
  return retval;
}

/* cat program */
/* cat [<] [infile] [|>] outfile */
int
cat(int retval, FILE *strmin, FILE *strmout, int fromcp)
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
  /* Shortest program here ever with the highest comment/code ratio */
  sync();
  /* This functions is always successful - listed as an error in sync(2) */
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
  close(0);
  close(1);
  close(2);

  /* change directory to root to avoid locking the device */
  chdir("/");

  while(1!=0)
  {
    _sync();
    sleep(30);
  }
}
