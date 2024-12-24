#include <stdio.h>
#include <stdlib.h>

/* OW does not seem to support *env[] arg to main */
extern char **environ;

int
main(int argc, char *argv[], char *env[])
{
  int count;

  puts("Commandline:");

  for (count = 0; count < argc; count++)
    printf("%d: %s\n", count, argv[count]);

  puts("\nEnvironment:");
  
  for (count = 0; environ[count] != NULL; count++)
    printf("%d: %s\n", count, environ[count]);

  putenv("PATH=");

  puts("\nEnvironment after a setenv (\"PATH=\");:");

  for (count = 0; environ[count]; count++)
    printf("%d: %s\n", count, environ[count]);

  return 0;
}
