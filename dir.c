/*
 *  DIR.C - dir internal command
 *
 *  Comments:
 *
 *  01/29/97 (Tim Norman)
 *    started.
 *
 *  06/13/97 (Tim Norman)
 *    Fixed code.
 *
 *  07/12/97 (Tim Norman)
 *    Fixed bug that caused the root directory to be unlistable
 *
 *  07/12/97 (Marc Desrochers)
 *    Changed to use maxx, maxy instead of findxy()
 *
 *  06/08/98 (Rob Lake)
 *    Added compatibility for /w in dir
 *
 * Compatibility for dir/s started 06/09/98 -- Rob Lake
 * 06/09/98 (Rob Lake)
 * -Tested that program finds directories off root fine
 *
 *
 * 06/10/98 (Rob Lake)
 *      -do_recurse saves the cwd and also stores it in Root
 *      -build_tree adds the cwd to the beginning of
 *   its' entries
 *      -Program runs fine, added print_tree -- works fine.. as EXE,
 *   program won't work properly as COM.
 *
 * 06/11/98 (Rob Lake)
 * -Found problem that caused COM not to work
 *
 * 06/12/98 (Rob Lake)
 *      -debugged...
 *      -added free mem routine
 *
 * 06/13/98 (Rob Lake)
 *      -debugged the free mem routine
 *      -debugged whole thing some more
 *      Notes:
 *      -ReadDir stores Root name and _Read_Dir does the hard work
 *      -PrintDir prints Root and _Print_Dir does the hard work
 *      -KillDir kills Root _after_ _Kill_Dir does the hard work
 *      -Integrated program into DIR.C(this file) and made some same
 *       changes throughout
 *
 * 06/14/98 (Rob Lake)
 *      -Cleaned up code a bit, added comments
 *
 * 06/16/98 (Rob Lake)
 * - Added error checking to my previously added routines
 *
 * 06/17/98 (Rob Lake)
 *      - Rewrote recursive functions, again! Most other recursive
 *      functions are now obsolete -- ReadDir, PrintDir, _Print_Dir,
 *      KillDir and _Kill_Dir.  do_recurse does what PrintDir did
 *      and _Read_Dir did what it did before along with what _Print_Dir
 *      did.  Makes /s a lot faster!
 *  - Reports 2 more files/dirs that MS-DOS actually reports
 *      when used in root directory(is this because dir defaults
 *      to look for read only files?)
 *      - Added support for /b, /a and /l
 *      - Made error message similar to DOS error messages
 * - Added help screen
 *
 * 06/20/98 (Rob Lake)
 * - Added check for /-(switch) to turn off previously defined
 * switches
 * - Added ability to check for DIRCMD in environment and
 * process it
 *
 * 06/21/98 (Rob Lake)
 * - Fixed up /B
 * - Now can dir *.ext/X, no spaces!
 *
 * 06/29/98 (Rob Lake)
 *      - error message now found in command.h
 *
 * 07/08/1998 (John P. Price)
 * - removed extra returns; closer to MSDOS
 * - fixed wide display so that an extra return is not displayed when
 *   there is five filenames in the last line.
 *
 * 07/12/98 (Rob Lake)
 * - Changed error messages
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 09-Aug-1998 (Rob Lake <rlake@cs.mun.ca>)
 * - fixed bug that caused blank lines to be printed
 * - renamed _Read_Dir to Read_Dir
 *
 * 10-Aug-1998 ska
 * - added checks for ^break
 *
 * 03-Dec-1998 John P Price
 * - Rewrote DIR command.  fixed problem with "DIR .COM" and other bugs
 *   Recursive switch does not work now, but it will be added soon.
 *
 * 31-Jan-1999 (John P. Price)
 * - Changed dir_print_header to use function INT21,AH=69 instead of the
 *   function it was using.  I'm not sure if this will fix anything or not.
 *   Also fixed bug with changing and restoring the current drive.
 *
 * 1999/04/23 ska
 * bugfix: cmd_dir(): dircmd can be NULL
 *
 * 25-Apr-1999 (John P. Price)
 * - changed dir so it always shows the bytes free.
 *
 * 29-Apr-1999 (John P. Price)
 * - Changed so that "dir command" with no extension will do as
 *   "dir command.*" (as it should).  Also made the display of the directory
 *   not include the whole filespec, but only the directory
 *
 * 2000/01/05 ska (Reported by Jeremy Greiner 2000/01/03)
 * bugfix: Last modification time displays year 2000 as 100
 *    --> Should be revised to allow 4-digit year
 *    Note: This fix differs from Jeremy Greiner's by that it does
 *    still display a 2-digit year rather than a 4-digit one.
 *    A four-digit year would break all batch files scanning the
 *    output of DIR.
 */

#include "config.h"

#ifdef INCLUDE_CMD_DIR

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dir.h>
#include <dirent.h>
#include <dos.h>
#include <io.h>
#include <conio.h>
#include <string.h>
#include <alloc.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "dfn.h"

#include "command.h"
#include "cmdline.h"
#include "strings.h"

/* useful macros */
#define MEM_ERR error_out_of_memory(); return 1;

#define WIDE_COLUMNS 5

static int optS, optP, optW, optB, optA, optL;

char *path;
unsigned line;
int need_nl;

#pragma argsused
optScanFct(opt_dir)
{ switch(ch) {
  case 'S': return optScanBool(optS);
  case 'P': return optScanBool(optP);
  case 'W': return optScanBool(optW);
  case 'B': return optScanBool(optB);
  case 'A': return optScanBool(optA);
  case 'L': return optScanBool(optL);
  }
  optErr();
  return E_Useage;
}

/*
 * pause
 *
 * pause until a key is pressed
 */
int pause(void)
{
  cmd_pause(NULL);

  return 0;
}

/*
 * incline
 *
 * increment our line if paginating, display message at end of screen
 */
int incline(void)
{

  if (!optP)
    return E_None;

  if (++line >= *maxy)
  {
    line = 0;
    return pause();
  }

  return 0;
}

int flush_nl(void)
{ if(need_nl) {
    putchar('\n');
    return incline();
  }

  return E_None;
}

/*
 * dir_print_header
 *
 * print the header for the dir command
 */
int dir_print_header(int drive)
{
  struct media_id
  {
    int info_level;
    int serial1;
    int serial2;
    char vol_id[11];
    char file_sys[8];
  }
  media;
  struct ffblk f;
  struct SREGS s;
  union REGS r;
  int disk;
  int rv;

  if (cbreak)
    return E_CBreak;

  if (optB)
    return 0;

  disk = getdisk();
  setdisk(drive);
  if (getdisk() != drive)
  {
    setdisk(disk);
    error_invalid_drive();
    return 1;
  }

  /* get the media ID of the drive */
/*
   Format of disk info:
   Offset  Size    Description     (Table 01766)
   00h    WORD    0000h (info level)
   02h    DWORD   disk serial number (binary)
   06h 11 BYTEs   volume label or "NO NAME    " if none present
   11h  8 BYTEs   (AL=00h only) filesystem type (see #01767)

 */

  r.x.ax = 0x6900;
  r.x.bx = drive + 1;
  s.ds = FP_SEG(&media);
  r.x.dx = FP_OFF(&media);
  int86x(0x21, &r, &r, &s);

  /* print drive info */
  printf("\n Volume in drive %c", drive + 'A');

  if (FINDFIRST("\\*.*", &f, FA_LABEL) == 0)
  {
    printf(" is %s\n", f.ff_name);
  }
  else
  {
    printf(" has no label\n");
  }

  setdisk(disk);

  if ((rv = incline()) == 0) {

  /* print the volume serial number if the return was successful */
  if (!r.x.cflag)
  {
    printf(" Volume Serial Number is %04X-%04X\n", media.serial2, media.serial1);
    rv = incline();
  }
  }

  return rv;
}

/*
 * convert
 *
 * insert commas into a number
 */
int convert(unsigned long num, char *des)
{
  char temp[32];
  int c = 0;
  int n = 0;

  assert(des);

  if (num == 0)
  {
    des[0] = '0';
    des[1] = 0;
    n = 1;
  }
  else
  {
    temp[31] = 0;
    while (num > 0)
    {
      if (((c + 1) % 4) == 0)
        temp[30 - c++] = ',';
      temp[30 - c++] = (char)(num % 10) + '0';
      num /= 10;
    }
    strcpy(des, &temp[31 - c]);
  }
  return n;
}

/*
 * print_summary: prints dir summary
 */
#pragma argsused
int print_summary(unsigned long files
  , unsigned long dirs
  , unsigned long bytes)
{
  char buffer[32];

  if (optB)
    return 0;

  convert(files, buffer);
  printf("   %6s file%c", buffer, files == 1 ? ' ' : 's');
  convert(bytes, buffer);
  printf("   %12s byte%c\n", buffer, bytes == 1 ? ' ' : 's');
  need_nl = 1;
//  printf("%9d dirs", dirs);
  return incline();
}

int print_total(unsigned long files, unsigned long dirs, unsigned long bytes)
{ int rv;

  if(optB)
    return 0;

  rv = flush_nl();
  if(rv == E_None) {
    printf("\tTotal of %s\n", path);
    if((rv = incline()) == E_None)
      return print_summary(files, dirs, bytes);
  }

  return rv;
}

int dir_print_free(void)
{
  char buffer[32];
  union REGS r;

  if(optB)
    return 0;

  /* print number of dirs and bytes free */
  r.h.ah = 0x36;
  r.h.dl = toupper(*path) - 'A' + 1;
  int86(0x21, &r, &r);
  convert((unsigned long)r.x.ax * r.x.bx * r.x.cx, buffer);
  printf(" %15s bytes free\n", buffer);
  return incline();
}

/*
 * dir_list
 *
 * list the files in the directory
 */
int dir_list(int pathlen
  , char *pattern
  , unsigned long *dcnt
  , unsigned long *fcnt
  , unsigned long *bcnt)
{
  struct ffblk file;
  unsigned long bytecount = 0;
  unsigned long filecount = 0;
  unsigned long dircount = 0;
  int time;
  int count;
  unsigned mode = FA_RDONLY | FA_ARCH | FA_DIREC;
  char *p;
  int rv = E_None;

  assert(path);
  assert(pattern);
  assert(pathlen >= 2);   /* at least root */

  if((p = realloc(path, pathlen + sizeof(file.ff_name) + 1)) == NULL) {
    error_out_of_memory();
    return E_NoMem;
  }
  path = p;

  /* if the user wants all files listed RL 06/17/98 */
  if (optA)
    mode |= FA_HIDDEN | FA_SYSTEM;

  /* Search for matching entries */
  path[pathlen - 1] = '\\';
  strcpy(&path[pathlen], pattern);
  if (FINDFIRST(path, &file, mode) == 0) {
  /* moved down here because if we are recursively searching and
   * don't find any files, we don't want just to print
   * Directory of C:\SOMEDIR
   * with nothing else
   */
   if(pathlen == 3)     /* root directory */
     path[pathlen] = '\0';    /* path := path w/o filename */
   else path[pathlen - 1] = '\0';
  if (!optB)
  {
    rv = flush_nl();
    if(rv == E_None) {
    printf(" Directory of %s\n", path);
    if((rv = incline()) == E_None) {
    putchar('\n');
    rv = incline();
  }
   }
  }

/* For counting columns of output */
  count = WIDE_COLUMNS;

  if(rv == E_None) do
  {
    assert(strlen(file.ff_name) < 13);

    if (cbreak)
      rv = E_CBreak;
    else {

    if (optL)
      strlwr(file.ff_name);

    if (optW)
    {
      char buffer[sizeof(file.ff_name) + 3];

      if (file.ff_attrib & FA_DIREC)
      {
        sprintf(buffer, "[%s]", file.ff_name);
        dircount++;
      }
      else
      {
        strcpy(buffer, file.ff_name);
        filecount++;
      bytecount += file.ff_fsize;
      }
      printf("%-15s", buffer);
      if (!--count)
      {
        /* outputted 5 columns */
        putchar('\n');
        rv = incline();
        count = WIDE_COLUMNS;
      }
    }
    else if (optB)
    {
      if (strcmp(file.ff_name, ".") == 0 || strcmp(file.ff_name, "..") == 0)
        continue;
      if (optS)
        fputs(path, stdout);
      printf("%-13s\n", file.ff_name);
      if (file.ff_attrib & FA_DIREC)
        dircount++;
      else {
        filecount++;
      bytecount += file.ff_fsize;
      }
      rv = incline();
    }
    else
    {
      char buffer[sizeof(long) * 4 + 2], *ext;

      if (file.ff_name[0] == '.')
        printf("%-13s", file.ff_name);
      else
      {
        ext = strrchr(file.ff_name, '.');
        if (!ext)
          ext = "";
        else
          *ext++ = '\0';

        printf("%-8s %-3s ", file.ff_name, ext);
      }

      if (file.ff_attrib & FA_DIREC)
      {
        printf("%-14s", "<DIR>");
        dircount++;
      }
      else
      {
        convert(file.ff_fsize, buffer);
        printf("   %10s ", buffer);
        bytecount += file.ff_fsize;
        filecount++;
      }

      printf("%.2d-%.2d-%02d", ((file.ff_fdate >> 5) & 0x000f),
             (file.ff_fdate & 0x001f), ((file.ff_fdate >> 9) + 80) % 100);
      time = file.ff_ftime >> 5 >> 6;
      printf("  %2d:%.2u%c\n",
             (time == 0 ? 12 : (time <= 12 ? time : time - 12)),
             ((file.ff_ftime >> 5) & 0x003f),
             (time <= 11 ? 'a' : 'p'));

      rv = incline();
    }
   }
  }
  while (rv == E_None && FINDNEXT(&file) == 0);
  }

/* Rob Lake, need to make clean output */
  if (rv == E_None && optW && (count != 0))
  {
    putchar('\n');
    rv = incline();
  }

  if (rv == E_None)
    if(filecount || dircount)
    {
    /* The code that was here is now in print_summary */
    rv = print_summary(filecount, dircount, bytecount);
    }
    else if(!optS)
    {
    error_file_not_found();
    rv = E_Other;
    }

  if(rv == E_None       /* no error */
   && optS            /* do recursively */
   && (pathlen == 3       /* root directory */
    || dircount > 2)) {     /* at least dir except . & .. */
      path[pathlen - 1] = '\\';
      strcpy(&path[pathlen], "*.*");
      if (FINDFIRST(path, &file, mode) == 0) do {
        if((file.ff_attrib & FA_DIREC) != 0 /* is directory */
         && strcmp(file.ff_name, ".") != 0  /* not cur dir */
         && strcmp(file.ff_name, "..") != 0) {  /* not parent dir */
        if (optL)
          strlwr(file.ff_name);
          strcpy(&path[pathlen], file.ff_name);
          rv = dir_list(pathlen + strlen(file.ff_name) + 1, pattern
           , &dircount, &filecount, &bytecount);
        }
      } while (rv == E_None && FINDNEXT(&file) == 0);

    if(rv == E_None) {
       if(pathlen == 3)     /* root directory */
         path[pathlen] = '\0';    /* path := path w/o filename */
       else path[pathlen - 1] = '\0';
      rv = print_total(filecount, dircount, bytecount);
     }
  }

    *dcnt += dircount;
    *fcnt += filecount;
    *bcnt += bytecount;

  return rv;
}

int dir_print_body(char *arg)
{ int rv;
  unsigned long dircount, filecount, bytecount;
  char *pattern, *cachedPattern;

    if((path = dfnexpand(arg, NULL)) == NULL) {
      error_out_of_memory();
      return E_NoMem;
    }

  dircount = filecount = bytecount = 0;

  /* print the header */
  if ((rv = dir_print_header(toupper(path[0]) - 'A')) == 0) {
    if(dfnstat(path) & DFN_DIRECTORY) {
      pattern = strchr(path, '\0');
      if(pattern[-1] != '\\')
        ++pattern;
      rv = dir_list(pattern - path, "*.*", &dircount, &filecount
     , &bytecount);
    } else {
    if((cachedPattern = strdup(pattern = dfnfilename(path))) == NULL) {
      error_out_of_memory();
      rv = E_NoMem;
    }
    else {
       rv = dir_list(pattern - path, cachedPattern, &dircount, &filecount
        , &bytecount);
       free(cachedPattern);
    }
   }
  }

  free(path);
  return rv;
}


/*
 * dir
 *
 * internal dir command
 */
#pragma argsused
int cmd_dir(char *rest)
{
  char **argv;
  int argc, opts;
  int rv;                       /* return value */

  /* initialize options */
  optS = optP = optW = optB = optA = optL = 0;

  /* read the parameters from env */
  if ((argv = scanCmdline(getEnv("DIRCMD"), opt_dir, NULL, &argc, &opts))
   == NULL)
    return 1;
  freep(argv);    /* ignore any parameter from env var */

  line = 0;
  /* read the parameters */
  if ((argv = scanCmdline(rest, opt_dir, NULL, &argc, &opts)) == NULL)
    return 1;

  if(argc)
    for(opts = 0
     ; opts < argc && (rv = dir_print_body(argv[opts])) == 0
     ; ++opts)
      ;
  else
    rv = dir_print_body("*.*");

  if(!rv)
    rv = dir_print_free();

  freep(argv);
  return rv;
}

#endif

#ifdef INCLUDE_CMD_VOL

/*
 * vol
 *
 * internal vol command
 */
#pragma argsused
int cmd_vol(char *rest)
{
  int drive;

  assert(rest);

  /* save the current directory info */
      line = 0;
      optB = 0;

  if (rest && *rest)
  {
    if ((strlen(rest) != 2) || (rest[1] != ':'))
    {
      displayString(TEXT_ERROR_INVALID_PARAMETER, rest);
      return E_Useage;
    }
  drive = toupper(rest[0]) - 'A';
  }
  else
    drive = getdisk();

  return dir_print_header(drive);
}
#endif
