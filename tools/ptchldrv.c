/*  $Id$

  Patch log drive

  This little utitilty patches the drive letter COMMAND.COM
  creates its log file on.

  2000/01/05 ska
  started as of implementation of the start logging feature
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

  /* Aquire LOG_FILE, the name of the logfile */
#include "../config.h"

static char logFilename[] = LOG_FILE;
static char hbuf[] = LOG_FILE;
static char buf[sizeof(logFilename) - 1];

main(int argc, char **argv)
{ long stringpos;
  char *fnam;
  FILE *f;
  int fill, ch;
  char ibuf[1024];

  if(argc > 2
   || argc == 2 && (strlen(argv[1]) != 1 || !isalpha(argv[1][0]))) {
    puts("Patches the drive letter FreeCom.COM creates its logfile on\nUsage: PTCHLDRV [drive_letter]");
    return 40;
  }

  if((f = fopen(fnam = "com.com", "r+b")) == NULL
   && (f = fopen(fnam = "com.exe", "r+b")) == NULL
   && (f = fopen(fnam = "command.com", "r+b")) == NULL) {
    puts("Cannot find COMMAND.COM");
    return 41;
  }
  printf("File \"%s\" successfully opened.\n", fnam);

  /* Search for the position of the logFilename within the image */
  stringpos = 0;
  fill = 0;
  do {
      /* fill the buffer */
    if(fill != sizeof(buf)
     && fread(buf + fill, sizeof(buf) - fill, 1, f) != 1)
        break;
    if(memcmp(logFilename + 1, buf, sizeof(buf)) == 0) {
      if(stringpos) {
        puts("The opened file contains the logfilename multiple times.\nThis file cannot be patched.");
        return 42;
      }
      stringpos = ftell(f);
      if(stringpos == -1L) {
        puts("General access error.\n");
        return 44;
      }
      stringpos -= sizeof(logFilename);
    }
      /* Read-in the next chunk */
    if((fnam = memchr(buf + 1, logFilename[1], sizeof(buf) - 1)) == NULL)
      fill = 0;
    else {
      fill = sizeof(buf) - (fnam - buf);
      memmove(buf, fnam, fill);
    }
  } while(1);

  if(!stringpos) {
    puts("The logfilename could not be located within the opened file.\nThis can happen, if FEATURE_CALL_LOGGING was disabled,\nwhen FreeCom was compiled.");
    return 43;
  }

  if(fseek(f, stringpos, SEEK_SET)) {
    puts("Cannot seek to logfilename.\n");
    return 45;
  }

  if(fread(hbuf, sizeof(hbuf), 1, f) != 1) {
    puts("Reading of logfilename failed.\n");
    return 46;
  }

  if(memcmp(hbuf + 1, logFilename + 1, sizeof(hbuf) - 1) != 0) {
    puts("Unexpected change in file.\n");
    return 47;
  }

  if(!isalpha(*hbuf)) {
    puts("Invalid drive letter within logfilename.\nFreeCom must be re-compiled.");
    return 50;
  }

  printf("Current drive letter the logfile is created on: %c\n", *hbuf);

  if(argc == 2) {
    printf("The drive letter shall be changed to: %c\n", ch = argv[1][0]);
    if(toupper(*hbuf) == toupper(ch)) {
      puts("No modification necessary.");
      return 0;
    }

    if(ferror(f)) {
      puts("General access error on file.");
      return 47;
    }

    printf("Shall the file be patched?\nIf you proceed, but the process fails,\nthe file must be replaced by a new copy!\n\nAnswer \"yes\" (all three characters) to proceed: ");
    if(fgets(ibuf, sizeof(ibuf), stdin) == NULL
     || ibuf[4] != '\0'
     || strcmp(ibuf, "yes\n") != 0) {
      puts("The input does not match \"yes\" --> Abort.\nThe file has not been modified.");
      return 60;
    }

    puts("\n\nProceed patching...");
    if(fseek(f, stringpos, SEEK_SET)) {
      puts("Cannot seek to logfilename.\n");
      return 48;
    }
    *logFilename = ch;

    if(fwrite(logFilename, 1, 1, f) != 1 || ferror(f)) {
      puts("Patching failed!\nThis file cannot be used no longer!");
      return 49;
    }

    fclose(f);

    puts("File patched successfully.");
  }

  return 0;
}
