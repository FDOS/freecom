#include <stdio.h>
#include <stdlib.h>
int
main(int argc, char *argv[], char *env[])
{
  int count;

  puts("Commandline:");

  for (count = 0; count < argc; count++)
    printf("%d: %s\n", count, argv[count]);

  puts("\nEnvironment:");

  for (count = 0; env[count]; count++)
    printf("%d: %s\n", count, env[count]);

  putenv("PATH=");

  puts("\nEnvironment after a setenv (\"PATH=\");:");

  for (count = 0; env[count]; count++)
    printf("%d: %s\n", count, env[count]);

  return 0;
}
