/* $Id$
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
 *  - Reports 2 more files/dirs that DOS actually reports
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
 *
 * 2000/07/07 Ron Cemer
 * Added code to detect a pattern of "." or "" and convert to ".\*.*",
 * to prevent "file not found" errors from command.com when executed
 * from the root of drive C: and "DIR" is typed without any arguments,
 * or if you type "DIR C:" and the current directory on drive C: is \.
 * Also added code to convert \.\ in paths to \ (eliminate the .\ when
 * not needed).  Hopefully this will help to bypass any findfirst/findnext
 * bugs which may exist in the kernel.
 *
 * 2000/07/16 Ron Cemer
 * Fixed "DIR .." or "DIR C:\FREEDOS\COM079\..".
 * Fixed "DIR /S".
 * No longer reallocate the "path" variable in dir_list().  This would break
 * "DIR /S" because dir_list() is recursive and simply tacks on additional
 * subdirectories to the end of the "path" variable.  So the "path" variable
 * must NOT be moving around in memory, so I pre-allocate it to 270 characters
 * to allow plenty of room to tack on subdirectories while recursing.
 * Changed formatting to exactly match DOS's formatting as much as possible,
 * except that the "bytes free" count is still printed in bytes instead of
 * KB or MB.
 *
 * 2001/02/16 ska
 * chg: using STRINGS resource
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
#if ( (!defined(__TURBOC__)) || (__TURBOC__ > 0x297) )
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
	/* FINDFIRST() file mask used if no /A has been given */
#define ATTR_DEFAULT FA_RDONLY | FA_ARCH | FA_DIREC
	/* FINDFIRST() file mask used if /A (no args) has been given */
#define ATTR_ALL FA_RDONLY | FA_ARCH | FA_DIREC | FA_HIDDEN | FA_SYSTEM;

/* Definitions for optO */
#define ORDER_BY_SIZE 2
#define ORDER_BY_DATE 4
#define ORDER_BY_NAME 8
#define ORDER_BY_EXT  0x10
#define ORDER_INVERSE 0x01
#define ORDER_DIRS_FIRST 0x20
#define ORDER_DIRS_LAST 0x40
#define ORDER_BY_MASK 0x1e    
#define DEFAULT_SORT_ORDER "NG"

unsigned char optOdir = 0;
char optOorderby[5];	/* Array: One byte longer than order methods,
							the last unused orderby[] byte is zero
							by design always
							--> eases implementation, e.g. memcpy()
							in scanOrder() and for() loop in
							orderFuntion() */


struct currDir {
	unsigned linecount; /* for /B */
	};          
	
static int optS, optP, optW, optB, optL, longyear, optO, dispLFN
	, lfnAvailable;
static struct ffblk _seg *orderArray;

static unsigned attrMay, attrMask, attrMatch;
/* within/right after the parsing of the options:
		what attributes "may" be on (but need not), which "must" be set
		and which "must not" be set.
	attrMay is just passed into FINDFIRST(),
	attrMask and attrMatch are aggregated so that
		<current attributes> & mask == match
	selects all valid files
*/


char *path;
unsigned line;
int need_nl;

struct LFNFindData {
  unsigned long attributes;
  unsigned long CreationTime[2];
  unsigned long AccesstionTime[2];
  unsigned long ModificationTime[2];
  unsigned long FileSize;
  unsigned long FileSizeHigh;
  unsigned char reserved[8];
  unsigned char FullName[260]; 
  unsigned char ShortName[14]; 
  } ;                     
struct { char x[sizeof(struct LFNFindData) == 0x13e ? 1 : -1]; } verify_size = {""};

void printLFNname(char *shortName, char *ext)
{
    char pathbuffer[128];
	struct REGPACK r;
	struct LFNFindData LFNEntry;
	int pathlen;
	
	/* Only few people disable this, right? */
	if (strchr(shortName,'~') == NULL)	/* ask for LFN only if necessary */
		return;
										/* reconstruct the path+filename */
	pathlen = strrchr(path,'\\') - path;

	sprintf(pathbuffer,"%*.*s\\%s%c%s"
	 , pathlen
	 , pathlen
	 , path
	 , shortName
	 , *ext ? '.' : 0x0
	 , ext);

/*	dprintf(("[LFN: path %s\n",pathbuffer)); */
	
	r.r_ax = 0x714e;
	r.r_cx = 0xff;
	r.r_si = 1;
	r.r_ds = FP_SEG(pathbuffer);
	r.r_dx = FP_OFF(pathbuffer);
	r.r_es = FP_SEG(&LFNEntry);
	r.r_di = FP_OFF(&LFNEntry);
	
	intr(0x21, &r);
	
	if(r.r_flags & 1) {
/*		dprintf(("[LFN: not supported %x]\n",r.r_ax)); */
		lfnAvailable = 0;
		return;
	}
	
	printf(" %.30s ", LFNEntry.FullName);

	r.r_bx = r.r_ax;	// close handle
	r.r_ax = 0x71a1;
	intr(0x21,&r);
}


/* The DIR command accepts more than one /A options, the later ones
	replaces former ones */
static scanAttr(const char *p)
{	unsigned attr;

	attrMask = attrMatch = 0;	/* purge previous /A*** */
	attrMay = ATTR_ALL;

	if(p && *p) {
		for(--p;;) {
			switch(toupper(*++p)) {
			case 'R': attr = FA_RDONLY; break;
			case 'A': attr = FA_ARCH; break;
			case 'D': attr = FA_DIREC; break;
			case 'H': attr = FA_HIDDEN; break;
			case 'S': attr = FA_SYSTEM; break;
			/* case 'L': WinXP has this one too? */
			/* case '+': */ case '-': continue;
			case '\0': goto done;
			default:	/* error */
				error_illformed_option(p);
				return E_Useage;
			}
			switch(p[-1]) {
			case '-': /* disable */	attrMatch &= ~attr; break;
			default: /* enable */	attrMatch |= attr; break;
			/* case '+': / * anyway * / break; */
			}
			attrMask |= attr;	/* "mustNot" is dropped later, must need to
									be ORed later anyway */
		}
done:
		/* no need to fetch entries with disabled attributes */
		attrMay &= ~attrMask | attrMatch;
	}
	/* else  no specifying arguments --> display all */

	return E_None;
}
    

    
    
static scanOrder(const char *p)
{
	if(!p || !*p)
		p = DEFAULT_SORT_ORDER;

restart:	
	memset(optOorderby, 0, sizeof(optOorderby));
	optOdir = 0;

	if(p && *p) {
		for(;;p++) {
			int inverse = p[-1] == '-';
			int option;
			int i;
			
			switch(toupper(*p)) {
			case '-': continue;
			case 'S': option = ORDER_BY_SIZE; break;
			case 'D': option = ORDER_BY_DATE; break;
			case 'N': option = ORDER_BY_NAME; break;
			case 'E': option = ORDER_BY_EXT; break;
			case 'G': optOdir = inverse? ORDER_DIRS_LAST: ORDER_DIRS_FIRST;
						continue;
			case 'U': ++p; goto restart;
			case '\0': goto done;
			default:	/* error */
				error_illformed_option(p);
				return E_Useage;
			}

			for(i = 0; i < sizeof(optOorderby); ++i) {
				if(optOorderby[i] == 0)
					break;
				if((optOorderby[i] & ORDER_BY_MASK) == option) {
					memcpy(optOorderby+i, optOorderby+i+1
					 , (sizeof(optOorderby)-1-i)*sizeof(optOorderby[0]));
				}
			}
			optOorderby[i] = option | inverse;
#ifdef DEBUG
			while(++i < sizeof(sizeof(optOorderby))) {
				assert(!optOorderby[i]);
			}
#endif
		}
done:;
	}

	optO = optOorderby[0] | optOdir;
	return E_None;
}

#pragma argsused
optScanFct(opt_dir)
{ switch(ch) {
  case 'S': return optScanBool(optS);
  case 'P': return optScanBool(optP);
  case 'W': return optScanBool(optW);
  case 'B': return optScanBool(optB);
  case 'O': if(!bool) return scanOrder(strarg);
  			break;
  case 'A': if(!bool) return scanAttr(strarg);
  			break;
  case 'L': return optScanBool(optL);
  case '4':
  case 'Y': return optScanBool(longyear);
  case 0:	/* Longname option, e.g. /A without argument sign */
  	switch(*optstr) {
  	case 'A': case 'a':		/* /A*** */
  		if(!bool && !optHasArg())
			return scanAttr(optstr + 1);
		break;
  	case 'O': case 'o':		/* /O*** */
  		if(!bool && !optHasArg())
			return scanOrder(optstr + 1);
		break;
    case 'L': case 'l':		/* enable to display LFNs? */
      if(optLong("LFN"))
        return optScanBool(dispLFN);
      break;
	}
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


  if (FINDFIRST("\\*.*", &f, FA_LABEL) == 0)
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
/*  union REGS r;
  struct SREGS sr; */
	struct REGPACK r;
  struct {
  	unsigned short whatever;
  	unsigned short version;
  	unsigned long  sectors_per_cluster; 
  	unsigned long  bytes_per_sector;
  	unsigned long  free_clusters;
  	unsigned long  total_clusters;
  	unsigned long  available_physical_sectors;
  	unsigned long  total_physical_sectors;
  	unsigned long  free_allocation_units; 
  	unsigned long  total_allocation_units; 
  	unsigned char  reserved[8];
  	} FAT32_Free_Space;
  static char rootname[] = "C:\\";	
  unsigned long clustersize;

  if(optB)
    return 0;

  /* print number of dirs and bytes free */

  convert(dirs, buffer);
  displayString(TEXT_DIR_FTR_DIRS, buffer);

  rootname[0] = toupper(*path);
  r.r_ax = 0x7303;
  r.r_ds = FP_SEG(rootname);
  r.r_dx = FP_OFF(rootname);
  r.r_es = FP_SEG(&FAT32_Free_Space);
  r.r_di = FP_OFF(&FAT32_Free_Space);
  r.r_cx = sizeof(FAT32_Free_Space);
  intr(0x21, &r);
  
  if(!(r.r_flags & 1)) {
	dprintf(("[DIR: Using FAT32 info]\n"));
	clustersize = FAT32_Free_Space.sectors_per_cluster
	 * FAT32_Free_Space.bytes_per_sector;
	
	if(FAT32_Free_Space.free_clusters >= 0x8000000l / clustersize) {
		convert(FAT32_Free_Space.free_clusters / 
				  ((1024l*1024) / clustersize), buffer);
		strcat(buffer, " Mega");
		goto output;
	}
  }  
  r.r_ax = 0x3600;
  r.r_dx = toupper(*path) - 'A' + 1;
  intr(0x21, &r);
  convert((unsigned long)r.r_ax * r.r_bx * r.r_cx, buffer);
output:
  displayString(TEXT_DIR_FTR_BYTES_FREE, buffer);

  return incline();
}

int DisplaySingleDirEntry(struct ffblk *file, struct currDir *cDir)
{
   int rv = E_None;

    if (cbreak)
      return E_CBreak;

    if (optL)
      strlwr(file->ff_name);

    if (optW)
    {
      char buffer[sizeof(file->ff_name) + 3];

      if (file->ff_attrib & FA_DIREC)
      {
        sprintf(buffer, "[%s]", file->ff_name);
      }
      else
      {
        strcpy(buffer, file->ff_name);
      }
      displayString(TEXT_DIR_LINE_FILENAME_WIDE, buffer);
      if (++cDir->linecount == WIDE_COLUMNS)
      {
        /* outputted 5 columns */
        putchar('\n');
        rv = incline();
        cDir->linecount = 0;
      }
    }
    else if (optB)
    {
      if (strcmp(file->ff_name, ".") == 0 || strcmp(file->ff_name, "..") == 0)
        return E_None;
      if (optS)
        fputs(path, stdout);
      displayString(TEXT_DIR_LINE_FILENAME_BARE, file->ff_name);
      rv = incline();
    }
    else
    {
      char buffer[sizeof(long) * 4 + 2], *ext;

      if (file->ff_name[0] == '.')
        displayString(TEXT_DIR_LINE_FILENAME_SINGLE, file->ff_name);
      else
      {
        ext = strrchr(file->ff_name, '.');
        if (!ext)
          ext = "";
        else
          *ext++ = '\0';

        displayString(TEXT_DIR_LINE_FILENAME, file->ff_name, ext);
      }

      if (file->ff_attrib & FA_DIREC)
      {
        displayString(TEXT_DIR_LINE_SIZE_DIR);
      }
      else
      {
        convert(file->ff_fsize, buffer);
        displayString(TEXT_DIR_LINE_SIZE, buffer);
      }

	{ char *p;
		int year, month, day;
		int hour, minute;

		year = (file->ff_fdate >> 9) + 80;
		if(longyear)
			year += 1900;
		else	year %= 100;
		day = file->ff_fdate & 0x001f;
		month = (file->ff_fdate >> 5) & 0x000f;
		hour = file->ff_ftime >> 5 >> 6;
		minute = (file->ff_ftime >> 5) & 0x003f;

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
		if(lfnAvailable)
			printLFNname(file->ff_name, ext);
		putchar('\n');
	 }

      rv = incline();
   }
   return cbreak? E_CBreak: rv;
}

int _Cdecl orderFunction(const void *p1, const void *p2)
{
  int i1 = *(int*)p1;
  int i2 = *(int*)p2;
  int rv = 0;
  int i;

  struct ffblk f1, f2;

  _fmemcpy(&f1, orderArray + i1 , sizeof(f1));
  _fmemcpy(&f2, orderArray + i2 , sizeof(f2));
  
  if(optOdir && (f1.ff_attrib & FA_DIREC) != (f2.ff_attrib & FA_DIREC))
  	return (optOdir & ORDER_DIRS_FIRST ? f1.ff_attrib: f2.ff_attrib)
  	         & FA_DIREC ? -1 : 1;  
  	         
  
  for (i = 0; rv == 0; i++) {
  	int opt = optOorderby[i];
  	assert(i < sizeof(optOorderby));
  	
	switch(opt & ORDER_BY_MASK) {
	
	  case 0:
	  	return rv;			/* is 0 actually */
	
	  case ORDER_BY_SIZE:
		if(f1.ff_fsize > f2.ff_fsize) rv = 1;
		else if(f1.ff_fsize < f2.ff_fsize) rv = -1;
		break;
	
	  case ORDER_BY_DATE:
	    rv = f1.ff_fdate - f2.ff_fdate;
	    if(!rv)
	    	 rv = f1.ff_ftime - f2.ff_ftime;
		break;
	
	  case ORDER_BY_EXT:
		{ char *x1 = strchr(f1.ff_name, '.');
		char *x2 = strchr(f2.ff_name, '.');
		
		
		if(!x1 && !x2)	/* both are equal */
			continue;
		
		if (x1 && x2) rv = strcmp(x1, x2);
		else if (!x1 && x2) rv = -1;
		else rv = 1;			/* x1 && !x2 */
	    }
	    break;
		
	  case ORDER_BY_NAME:
		rv = strcmp(f1.ff_name,f2.ff_name);
		break;
	  }
	  if (opt & ORDER_INVERSE) rv = - rv;
	}  

  return rv;
}    	


int flushOrder(struct ffblk _seg *orderArray,
  		int *orderIndex,
  		int  orderCount)
{
  int i;
  struct ffblk file;
  struct currDir cDir = {0};
  
  for (i = 0; i < orderCount; i++)
  	orderIndex[i] = i;
  	
  qsort(orderIndex, orderCount, sizeof(orderIndex[0]), orderFunction);

  for (i = 0; i < orderCount; i++) {
  	int rv;
    _fmemcpy(&file, &orderArray[orderIndex[i]], sizeof(file));
    if((rv = DisplaySingleDirEntry(&file, &cDir)) != E_None)
    	return rv;
  }
  return E_None;  
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
  int rv = E_None;
  struct currDir cDir = {0};
  
#define MAX_ORDER (0xffff / sizeof(struct ffblk))
  int *orderIndex;
  int  orderCount;

  assert(path);
  assert(pattern);
  assert(pathlen >= 2);   /* at least root */

  if(optO)  {
    orderIndex = malloc(MAX_ORDER * sizeof(unsigned));
    if(!orderIndex) {
    	error_out_of_memory();
    	optO = 0;
	} else {
		orderArray = (void _seg*)DOSalloc(0x1000,0);
		if(!orderArray) {
			free(orderIndex);
			error_out_of_dos_memory();
			optO = 0;
		}    	
		orderCount = 0;
  	}
  }
    
  /* Search for matching entries */
  path[pathlen - 1] = '\\';
  strcpy(&path[pathlen], pattern);

  if (FINDFIRST(path, &file, attrMay) == 0) {
  	int printDirectoryEntry = !optB;
/* For counting columns of output */
  cDir.linecount = 0;
  /* if optB && optS the path with trailing backslash is needed,
  	also for optS below do {} while */
  strcpy(&path[pathlen - 1], "\\");

  if(rv == E_None) do
  if((file.ff_attrib & attrMask) == attrMatch) {
    assert(strlen(file.ff_name) < 13);

  /* moved down here because if we are recursively searching and
   * don't find any files, we don't want just to print
   * Directory of C:\SOMEDIR
   * with nothing else
   * 2003-02-03 moved down even further to handle attribute selection.
   */

  if(printDirectoryEntry) {
    printDirectoryEntry = 0;
    rv = flush_nl();
    if(rv == E_None) {
	   	/* path without superflous '\' at its end */
	   if(pathlen == 3)     /* root directory */
		 path[pathlen] = '\0';    /* path := path w/o filename */
	   else path[pathlen - 1] = '\0';
        displayString(optS ? TEXT_DIR_DIRECTORY: TEXT_DIR_DIRECTORY_WITH_SPACE
         , path);
	    path[pathlen - 1] = '\\';	/* need this below */
        if((rv = incline()) == E_None) {
        putchar('\n');
        rv = incline();
    }
   }
  }

    if (cbreak)
      rv = E_CBreak;
    else if(rv == E_None) {
		if(file.ff_attrib & FA_DIREC) {
			dircount++;
		} else {
			filecount++;
			bytecount += file.ff_fsize;
		}
		if(optO) {  
			_fmemcpy(&orderArray[orderCount], &file, sizeof(file));
			orderCount++;
			if(orderCount >= MAX_ORDER) {
				rv = flushOrder(orderArray,orderIndex,orderCount);
				orderCount = 0;
			}
		} else
			rv = DisplaySingleDirEntry(&file, &cDir);
    }
  }
  while (rv == E_None && FINDNEXT(&file) == 0);
  }

  if(optO) {
    if(rv == E_None)
		rv = flushOrder(orderArray,orderIndex,orderCount);
    free(orderIndex);
    DOSfree(FP_SEG(orderArray));
  }

  if (rv == E_None && optW && (cDir.linecount != 0)) {
    putchar('\n');
    rv = incline();
  }

	if(rv == E_None)
		if(filecount || dircount)
			rv = print_summary(filecount, bytecount);
		else if(!optS) {
			error_file_not_found();
			rv = E_Other;
		}

  if(rv == E_None       /* no error */
   && optS) {            /* do recursively */
      /* already set for optB && optS before do {} while above 
		  path[pathlen - 1] = '\\';		*/
      strcpy(&path[pathlen], "*.*");
      	/* Import attributes S & H from "maybe" */
      if (FINDFIRST(path, &file, attrMay | FA_DIREC) == 0) do {
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
      } while (rv == E_None && FINDNEXT(&file) == 0);
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
	if((p = abspath(arg, 1)) == 0) {
		return E_NoMem;
	}
	if((path = realloc(p, 270*sizeof(char))) == 0) {
		free(p);
		error_out_of_memory();
		return E_NoMem;
	}

	filecount = bytecount = 0;
	lfnAvailable = dispLFN;	/* reset the LFN display flag for this
		argument, maybe the drive is changed */

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

  if(optS) {
    if(filecount)
		rv = print_total(filecount, bytecount);
    else {
		error_file_not_found();
		rv = E_Other;
    }
  }
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
  int appState;					/* DOS/APPEND state */

  /* initialize options */
  attrMask = attrMatch = optO = dispLFN
   = longyear = optS = optP = optW = optB = optL = 0;
  attrMay = ATTR_DEFAULT;

  /* read the parameters from env */
  if ((argv = scanCmdline(getEnv("DIRCMD"), opt_dir, 0, &argc, &opts))
   == 0)
    return 1;
  freep(argv);    /* ignore any parameter from env var */

  line = 0;
  /* read the parameters */
  if ((argv = scanCmdline(rest, opt_dir, 0, &argc, &opts)) == 0)
    return 1;

  appState = appendDisable();
  dircount = 0;
  if(argc)
    for(opts = 0
     ; opts < argc && (rv = dir_print_body(argv[opts], &dircount)) == 0
     ; ++opts)
      ;
  else
    rv = dir_print_body(".", &dircount);

  appendRestore(appState);
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
