/*
 *  DIR.C - dir internal command
 *
 */

#include "../config.h"

#include <assert.h>
#include <conio.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Not available with TURBOC++ 1.0 or earlier: */
#ifdef _TC_LATER_
#include <dirent.h>
#endif

#include <dfn.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../err_fcts.h"
#include "../include/nls.h"
#include "../strings.h"

/* useful macros */
#define MEM_ERR error_out_of_memory(); return 1;

#define WIDE_COLUMNS 5

static int optS, optP, optW, optB, optA, optL, longyear;

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
  case 'Y': return optScanBool(longyear);
  }
  optErr();
  return E_Useage;
}

/*
 * pause
 *
 * pause until a key is pressed
 */
static int pause(void)
{
  cmd_pause(0);

  return 0;
}

/*
 * incline
 *
 * increment our line if paginating, display message at end of screen
 */
static int incline(void)
{

  if (!optP)
    return E_None;

  if (++line >= MAX_Y)
  {
    line = 0;
    return pause();
  }

  return 0;
}

static int flush_nl(void)
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
static int dir_print_header(int drive)
{
	/* one byte alignment */
#pragma -a-
  struct media_id
  {
    int info_level;
    int serial1;
    int serial2;
    char vol_id[11];
    char file_sys[8];
  }
  media;
	/* standard alignment */
#pragma -a.
  struct ffblk f;
  struct SREGS s;
  union REGS r;
  int currDisk;
  int rv;

  if (cbreak)
    return E_CBreak;

  if (optB)
    return 0;

  currDisk = getdisk();
  if(changeDrive(drive+1) != 0) {
    setdisk(currDisk);
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
 	displayString(TEXT_DIR_HDR_VOLUME, drive + 'A');


  if(findfirst("\\*.*", &f, FA_LABEL) == 0)
  {
        /* Added to remove "." from labels which are longer than
               8 characters (as DOS does). */
    char *dotptr = strchr(f.ff_name, '.');
    if (dotptr != 0)
    	if(strlen(dotptr + 1))
			memmove(dotptr, dotptr + 1, strlen(dotptr));
		else *dotptr = '\0';		/* dot at end of name */
    displayString(TEXT_DIR_HDR_VOLUME_STRING, f.ff_name);
  }
  else
  {
    displayString(TEXT_DIR_HDR_VOLUME_NONE);
  }

  setdisk(currDisk);

  if ((rv = incline()) == 0) {
  /* print the volume serial number if the return was successful */
	  if (!r.x.cflag) {
		displayString(TEXT_DIR_HDR_SERIAL_NUMBER
		 , media.serial2, media.serial1);
		rv = incline();
	  }
  }

        /* Added to exactly match DOS's formatting. */
  if ( (optS) && (rv == 0) ) {
      putchar('\n');
      rv = incline();
  }

  return rv;
}


#ifdef INCLUDE_CMD_DIR
/*
 * print_summary: prints dir summary
 */
static int print_summary(unsigned long files
  , unsigned long bytes)
{
  char buffer[32];

  if (optB)
    return 0;

  convert(files, buffer);
  displayString(TEXT_DIR_FTR_FILES, buffer);
  convert(bytes, buffer);
  displayString(TEXT_DIR_FTR_BYTES, buffer);
  need_nl = 1;
  return incline();
}

static int print_total
    (unsigned long files,
     unsigned long bytes)
{ int rv;

  if(optB)
    return 0;

  rv = flush_nl();
  if(rv == E_None) {
    displayString(TEXT_DIR_FTR_TOTAL_NUMBER);
    if((rv = incline()) == E_None)
      return print_summary(files, bytes);
  }

  return rv;
}

static int dir_print_free(unsigned long dirs)
{
  char buffer[32];
  union REGS r;

  if(optB)
    return 0;

  /* print number of dirs and bytes free */

  convert(dirs, buffer);
  displayString(TEXT_DIR_FTR_DIRS, buffer);

  r.h.ah = 0x36;
  r.h.dl = toupper(*path) - 'A' + 1;
  int86(0x21, &r, &r);
  convert((unsigned long)r.x.ax * r.x.bx * r.x.cx, buffer);
  displayString(TEXT_DIR_FTR_BYTES_FREE, buffer);

  return incline();
}

/*
 * dir_list
 *
 * list the files in the directory
 */
static int dir_list(int pathlen
  , char *pattern
  , unsigned long *dcnt
  , unsigned long *fcnt
  , unsigned long *bcnt
  )
{
  struct ffblk file;
  unsigned long bytecount = 0;
  unsigned long filecount = 0;
  unsigned long dircount = 0;
  int count;
  unsigned mode = FA_RDONLY | FA_ARCH | FA_DIREC;
  int rv = E_None;

  assert(path);
  assert(pattern);
  assert(pathlen >= 2);   /* at least root */

  /* if the user wants all files listed */
  if (optA)
    mode |= FA_HIDDEN | FA_SYSTEM;

  /* Search for matching entries */
  path[pathlen - 1] = '\\';
  strcpy(&path[pathlen], pattern);

  if(findfirst(path, &file, mode) == 0) {
  /* moved down here because if we are recursively searching and
   * don't find any files, we don't want just to print
   * Directory of C:\SOMEDIR
   * with nothing else
   */

  if (!optB)
  {
    rv = flush_nl();
    if(rv == E_None) {
	   	/* path without superflous '\' at its end */
	   if(pathlen == 3)     /* root directory */
		 path[pathlen] = '\0';    /* path := path w/o filename */
	   else path[pathlen - 1] = '\0';
        displayString(optS ? TEXT_DIR_DIRECTORY: TEXT_DIR_DIRECTORY_WITH_SPACE
         , path);
        if((rv = incline()) == E_None) {
        putchar('\n');
        rv = incline();
    }
   }
  }

/* For counting columns of output */
  count = WIDE_COLUMNS;
  /* if optB && optS the path with trailing backslash is needed,
  	also for optS below do {} while */
  strcpy(&path[pathlen - 1], "\\");

  if(rv == E_None) do {
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
      displayString(TEXT_DIR_LINE_FILENAME_WIDE, buffer);
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
      displayString(TEXT_DIR_LINE_FILENAME_BARE, file.ff_name);
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
        displayString(TEXT_DIR_LINE_FILENAME_SINGLE, file.ff_name);
      else
      {
        ext = strrchr(file.ff_name, '.');
        if (!ext)
          ext = "";
        else
          *ext++ = '\0';

        displayString(TEXT_DIR_LINE_FILENAME, file.ff_name, ext);
      }

      if (file.ff_attrib & FA_DIREC)
      {
        displayString(TEXT_DIR_LINE_SIZE_DIR);
        dircount++;
      }
      else
      {
        convert(file.ff_fsize, buffer);
        displayString(TEXT_DIR_LINE_SIZE, buffer);
        bytecount += file.ff_fsize;
        filecount++;
      }

	{ char *p;
		int year, month, day;
		int hour, minute;

		year = (file.ff_fdate >> 9) + 80;
		if(longyear)
			year += 1900;
		else	year %= 100;
		month = file.ff_fdate & 0x001f;
		day = (file.ff_fdate >> 5) & 0x000f;
		hour = file.ff_ftime >> 5 >> 6;
		minute = (file.ff_ftime >> 5) & 0x003f;

		p = nls_makedate(0, year, month, day);
		if(!p) {
			error_out_of_memory();
			return E_NoMem;
		}
		putchar(' ');
		fputs(p, stdout);
		free(p);
		p = nls_maketime(NLS_MAKE_SHORT_AMPM, hour, minute, -1, 0);
		if(!p) {
			error_out_of_memory();
			return E_NoMem;
		}
		putchar(' ');
		fputs(p, stdout);
		free(p);
		putchar('\n');
	 }

      rv = incline();
    }
   }
  }
  while (rv == E_None && findnext(&file) == 0);
  }

  if (rv == E_None && optW && (count != 0))
  {
    putchar('\n');
    rv = incline();
  }

  if (rv == E_None)
    if(filecount || dircount)
    {
    /* The code that was here is now in print_summary */
    rv = print_summary(filecount, bytecount);
    }
    else if(!optS)
    {
    error_file_not_found();
    rv = E_Other;
    }

  if(rv == E_None       /* no error */
   && optS) {            /* do recursively */
      /* already set for optB && optS before do {} while above 
		  path[pathlen - 1] = '\\';		*/
      strcpy(&path[pathlen], "*.*");
      if (findfirst(path, &file, mode) == 0) do {
        if((file.ff_attrib & FA_DIREC) != 0 /* is directory */
         && strcmp(file.ff_name, ".") != 0  /* not cur dir */
         && strcmp(file.ff_name, "..") != 0) {  /* not parent dir */
        if (optL)
          strlwr(file.ff_name);
          strcpy(&path[pathlen], file.ff_name);
          rv = dir_list(pathlen + strlen(file.ff_name) + 1, pattern
           , &dircount, &filecount, &bytecount
           );
        }
      } while (rv == E_None && findnext(&file) == 0);
  }

    *dcnt += dircount;
    *fcnt += filecount;
    *bcnt += bytecount;

  return rv;
}

static int dir_print_body(char *arg, unsigned long *dircount)
{	int rv;
	unsigned long filecount, bytecount;
	char *pattern, *cachedPattern;
	char *p;

		/* Modified to pre-allocate path to 270 bytes so that
		   we don't have to realloc() it later.  That was causing
		   "DIR /S" not to work properly.  The path variable cannot
		   be reallocated once dir_list() is called, because dir_list()
		   is recursive.  This will also help to reduce memory
		   fragmentation. */
	if((p = dfnfullpath(arg)) == 0) {
		error_out_of_memory();
		return E_NoMem;
	}
	if((path = erealloc(p, 270*sizeof(char))) == 0) {
		free(p);
		return E_NoMem;
	}

	filecount = bytecount = 0;

	/* print the header */
	if((rv = dir_print_header(toupper(path[0]) - 'A')) == 0) {
		/* There are some directory specs that are not detected by
			dfnstat() as they are no part of the filesystem in DOS */
		pattern = dfnfilename(path);
		assert(p);
		if(!*pattern || (dfnstat(path) & DFN_DIRECTORY) != 0) {
			pattern = strchr(pattern, '\0');
			if(pattern[-1] != '\\')
				++pattern;
			rv = dir_list(pattern - path, "*.*", dircount, &filecount
			 , &bytecount
			 );
		} else {
			if((cachedPattern = strdup(pattern)) == 0) {
				error_out_of_memory();
				rv = E_NoMem;
			} else {
				rv = dir_list(pattern - path, cachedPattern, dircount
				 , &filecount, &bytecount
				 );
				free(cachedPattern);
			}
		}
	}

  if(optS)
  	rv = print_total(filecount, bytecount);
  if(!rv)
    rv = dir_print_free(*dircount);

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
  unsigned long dircount;

  /* initialize options */
  longyear = optS = optP = optW = optB = optA = optL = 0;

  /* read the parameters from env */
  if ((argv = scanCmdline(getEnv("DIRCMD"), opt_dir, 0, &argc, &opts))
   == 0)
    return 1;
  freep(argv);    /* ignore any parameter from env var */

  line = 0;
  /* read the parameters */
  if ((argv = scanCmdline(rest, opt_dir, 0, &argc, &opts)) == 0)
    return 1;

  dircount = 0;
  if(argc)
    for(opts = 0
     ; opts < argc && (rv = dir_print_body(argv[opts], &dircount)) == 0
     ; ++opts)
      ;
  else
    rv = dir_print_body(".", &dircount);

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
      error_syntax(rest);
      return E_Useage;
    }
  drive = toupper(rest[0]) - 'A';
  }
  else
    drive = getdisk();

  return dir_print_header(drive);
}

#endif
