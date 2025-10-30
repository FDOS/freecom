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
#include <dos.h>
#ifndef FA_DEVICE
#define FA_DEVICE 0x0040u
#endif
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <portable.h>

/* Not available with TURBOC++ 1.0 or earlier: */
#if defined __GNUC__
#define pause cmdpause
#elif ( (!defined(__WATCOMC__) && !defined(__TURBOC__)) || (__TURBOC__ > 0x297) )
#include <dirent.h>
#endif

#include "dfn.h"

#define __LFNFUNCS_C
#include "../include/lfnfuncs.h"
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

static unsigned char optOdir = 0;
static char optOorderby[5]; /* Array: One byte longer than order methods,
							the last unused orderby[] byte is zero
							by design always
							--> eases implementation, e.g. memcpy()
							in scanOrder() and for() loop in
							orderFuntion() */


struct currDir {
	unsigned linecount; /* for /B */
	};          
	
static int optS, optP, optW, optB, optL, longyear, optO;


#ifdef FEATURE_LONG_FILENAMES
static int dispLFN;
#endif

#ifdef FEATURE_DESCRIPT_ION
static int descriptionExists;
static int fDescription;
static void showDescription(const char *shortName, char *ext);
#endif

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


static char *path;
static unsigned line;
static int need_nl;

/* FAT32: file sizes are less then unsigned long, but accumulated
          directory sizes may be up to 2 TB

   so here comes simple 'large integer' arithmetic with billions separate
*/

typedef struct _bignum
{
  unsigned long low;
  unsigned billions;
} bignum;

static void bignum_add(bignum *big, bignum *to_add)
{
  big->billions += to_add->billions;
  big->low  += to_add->low;

  if (big->low >= 1000000000ul) {	/* carry propagation, school style */
    big->low -= 1000000000ul;
    big->billions++;
  }
}

#ifdef FEATURE_LONG_FILENAMES
static void printLFNname(char *shortName, char *ext)
{
    IREGS r;
    char pathbuffer[128];
	char longname[270];
	int pathlen;

	/* reconstruct the path+filename */
	pathlen = strrchr(path,'\\') - path;

	sprintf(pathbuffer,"%*.*s\\%s%c%s"
	 , pathlen
	 , pathlen
	 , path
	 , shortName
	 , *ext ? '.' : 0x0
	 , *ext ? ext : 0x0);

    dprintf(("[LFN: path %s\n",pathbuffer)); 
	
      /* LFN get canonical LFN */
	r.r_flags = 1;	/* CY before 21.71 calls! */
	r.r_ax = 0x7160;
	r.r_cx = 0x8002;
	r.r_si = FP_OFF( pathbuffer );
    r.r_ds = FP_SEG( pathbuffer );
	r.r_di = FP_OFF( longname );
    r.r_es = FP_SEG( longname );
	intrpt(0x21, &r);
	
    if( r.r_flags & 1 || r.r_ax == 0x7100 || !__supportlfns ) {
        dprintf(("[LFN: not supported %x]\n",r.r_ax)); 
		return;
	}

    if(samefile(longname, ".")) strcpy(longname, "\\.");

    if(dfnstat("..") != 0 && samefile(longname, "..")) strcpy(longname, "\\..");
	
	printf(" %.30s ", &strrchr(longname, '\\')[1]);
}
#endif


#ifdef FEATURE_DESCRIPT_ION
static int incline(void); /* forward reference */

/* this will read in next non-blank line from fDescription,
   and stores next token (ie filename or description).
   returns 0 on any error or no more entries in file
   *buf will equal '\0' or successfully read in filename
*/
static int descGetNextToken(char *buf, int fn, int maxlen)
{
  /* skip blank lines and any initial spaces, also exit early on any read error */
  do {
    if (dos_read(fDescription, buf, 1) != 1)
    {
      /*printf("[read error]\n");*/
      *buf = '\0';
      return 0;
    }
  } while ( (*buf == ' ') || (fn && isspace(*buf)) );

  /* read in characters until end of line/file (or space) is found */
  do {
    if (maxlen > 1) /* reuse last char in buffer, but don't exceed maxlen */
    {
      maxlen--;
      buf++;
    }
    if (dos_read(fDescription, buf, 1) != 1)
      break;
  } while ( (*buf!='\r') && (*buf!='\n') && 
            (*buf!=0x4/*Ctrl-D*/) && (*buf!=26/*Ctrl-Z*/) &&
            (!fn || (fn && !isspace(*buf))) );

  /* if read in description, then skip to end of line */
  /* note: because we stop at \r, next read may seem like blank line (\n) */
  if (!fn) 
    while ( (*buf!='\r') && (*buf!='\n') )
    {
      if (dos_read(fDescription, buf, 1) != 1)
        break;
    }

  /* terminate filename read in */
  *buf = '\0';
  return 1;
}

/* if a DESCRIPT.ION file was found (and opened) in current
   directory, then sequentially (as it's unsorted) read through it
   and print matching descriptions if found. Basic format are lines of
   filename <whitespace> description text [Ctrl-D ...] <EOL (\r,\n,\r\n)>|<EOF>
 */
static void showDescription(const char *shortName, char *ext)
{
  char fn[14], dummy[1], buf[128]; /* 4096 is max line size officially supported */
  assert(fDescription != 0);

  if (*shortName == '.') return; /* ignore . and .. entries */

  sprintf(fn, "%s%c%s", shortName, (*ext)?'.':0x0, ext); /* fn is 8.3 filename[.extension] */
  fn[13]='\0';
  lseek(fDescription, 0, SEEK_SET);  /* start at beginning of unsorted description file */
  while (descGetNextToken(buf,1,sizeof(buf)) && (strcmpi(fn,buf) != 0)) 
    { descGetNextToken(dummy,0,sizeof(dummy)); /* skip rest of this line & try again */ }

  if (*buf) /* found line in DESCRIPT.ION file matching this file */
  {
    descGetNextToken(buf,0,sizeof(buf));
    /* loop displaying upto 34 characters of description at a time. */
    #define DESCLEN 34
    {
      char *p = buf;
      while (strlen(p) > DESCLEN) /* display in chunks */
      {
        char t;

        /* start at end of chunk, try to split word at whitespace [or
           any punctuation]; don't split if word too big.
           i set to where '\0' will be stored.
         */
        int i = DESCLEN+1;
        while ((i>(DESCLEN/2)) && !isspace(*(p+i)) /*&& !ispunct(*(p+i))*/) i--;
        if (i<=(DESCLEN/2)) i = DESCLEN+1;
        else if (i<=DESCLEN) i++;

        t = *(p+i);                   /* store character          */
        *(p+i) = '\0';                /* temp force end of string */
        printf("\n");  incline();     /* handle dir /p            */
        printf("%44c%s", ' ', p);     /* display the substring    */
        p += i;                       /* skip past portion shown  */
        *p = t;                       /* restore saved character  */

        while(isspace(*p)) p++;       /* skip past whitespace splitting words at line edge */
      }
      /* and print the final (or only) chunk of description */
      printf("\n");  incline();       /* handle dir /p            */
      printf("%44c%s", ' ', p);
      /* printf("\n%44c%s", ' ', (*buf)? buf : "<empty description>"); */
    }
  }
  /* else printf("\n%44c%s", ' ', "NO MATCH FOUND"); */
}
#endif


/* The DIR command accepts more than one /A options, the later ones
	replaces former ones */
static int scanAttr(const char *p)
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
    

    
    
static int scanOrder(const char *p)
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

optScanFct(opt_dir)
{
  (void)arg;
  switch(ch) {
  case 'S': return optScanBool(optS);
  case 'P': return optScanBool2(optP);  /* multiple uses, /P /P, do not cancel, only /-P */
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
#ifdef FEATURE_LONG_FILENAMES
    case 'L': case 'l':		/* enable to display LFNs? */
      if(optLong("LFN"))
        return optScanBool(dispLFN);
      break;
#endif
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
    outc('\n');
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

# include "algnbyte.h"

  struct media_id
  {
    int info_level;
    int serial1;
    int serial2;
    char vol_id[11];
    char file_sys[8];
  }
  media;

# include "algndflt.h"

  struct ffblk f;
  IREGS r;
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

  r.r_ax = 0x6900;
  r.r_bx = drive + 1;
  r.r_ds = FP_SEG(&media);
  r.r_dx = FP_OFF(&media);
  intrpt( 0x21, &r );

  /* print drive info */
 	displayString(TEXT_DIR_HDR_VOLUME, drive + 'A');


  if (sfnfindfirst("\\*.*", &f, FA_LABEL) == 0)
  {
        /* Added to remove "." from labels which are longer than
           8 characters (as DOS does), but must pad name with spaces. */
    char *dotptr = strchr(f.ff_name, '.');
    if (dotptr != 0)
    {
      char *mptr = f.ff_name+8;
      /* shift extension either back over dot or to end of space padding */
      memmove(mptr, dotptr+1, 4);  /* 4=max 3 char ext + \0 */
      /* add spaces for padding / overwrite with spaces shifted extension */
      while (dotptr < mptr)
        *dotptr++ = ' ';  /* pad with space and increment */
    }
    displayString(TEXT_DIR_HDR_VOLUME_STRING, f.ff_name);
  }
  else
  {
    displayString(TEXT_DIR_HDR_VOLUME_NONE);
  }

  setdisk(currDisk);

  if ((rv = incline()) == 0) {
  /* print the volume serial number if the return was successful */
      if (!( r.r_flags & 1 )) {
		displayString(TEXT_DIR_HDR_SERIAL_NUMBER
		 , media.serial2, media.serial1);
		rv = incline();
	  }
  }

        /* Added to exactly match DOS's formatting. */
  if ( (optS) && (rv == 0) ) {
      outc('\n');
      rv = incline();
  }

  return rv;
}


#ifdef INCLUDE_CMD_DIR
/*
 * print_summary: prints dir summary
 */
static int print_summary(unsigned long files
  , bignum *bytes)
{
  char buffer[32];

  if (optB)
    return 0;

  convert(files, 0, buffer);
  displayString(TEXT_DIR_FTR_FILES, buffer);
  convert(bytes->low, bytes->billions, buffer);
  displayString(TEXT_DIR_FTR_BYTES, buffer);
  need_nl = 1;
  return incline();
}

static int print_total
    (unsigned long files,
     bignum *bytes)
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
  IREGS r;
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

  convert(dirs, 0, buffer);
  displayString(TEXT_DIR_FTR_DIRS, buffer);

  rootname[0] = toupper(*path);
  r.r_flags = 1;	/* CY before 21.73 calls! */
  r.r_ax = 0x7303;
  r.r_ds = FP_SEG(rootname);
  r.r_dx = FP_OFF(rootname);
  r.r_es = FP_SEG(&FAT32_Free_Space);
  r.r_di = FP_OFF(&FAT32_Free_Space);
  r.r_cx = sizeof(FAT32_Free_Space);
  intrpt( 0x21, &r);

  /* Note: RBIL carry clear and al==0 also means unimplemented 
     alternately carry set and ax==undefined (usually unchanged) for unimplemented
     ecm: RBIL is wrong, CF unchanged al=0 is the typical error return.
     EDR-DOS returns NC ax=0 so checking for al!=0 here was wrong.
  */
  if(!( r.r_flags & 1 ) && ( r.r_ax != 0x7300 ) ) {
	dprintf(("[DIR: Using FAT32 info]\n"));
	clustersize = FAT32_Free_Space.sectors_per_cluster
	 * FAT32_Free_Space.bytes_per_sector;

       /* The following loop is intended to handle large free space amounts
          and huge [fake] cluster sizes, such as NTFS4DOS driver on large HDs.
       */
       if (clustersize)
          if (FAT32_Free_Space.free_clusters >= 0x80000000ul / clustersize)
                {
                int shift;
                
                for (shift = 21; --shift;)
                        {
                        if ((clustersize & 1) == 0) clustersize >>= 1;
                        else                        FAT32_Free_Space.free_clusters >>= 1;
                        }
                
                convert(FAT32_Free_Space.free_clusters * clustersize, 0, buffer);

                strcat(buffer, " Mega");
                goto output;
                }
  }  
  r.r_ax = 0x3600;
  r.r_dx = toupper(*path) - 'A' + 1;
  intrpt(0x21, &r);
  convert((unsigned long)r.r_ax * r.r_bx * r.r_cx, 0, buffer);
output:
  displayString(TEXT_DIR_FTR_BYTES_FREE, buffer);

  return incline();
}

static int DisplaySingleDirEntry(struct ffblk *file, struct currDir *cDir)
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
        outc('\n');
        rv = incline();
        cDir->linecount = 0;
      }
    }
    else if (optB)
    {
      if (strcmp(file->ff_name, ".") == 0 || strcmp(file->ff_name, "..") == 0)
        return E_None;
      if (optS)
        outs(path);
      displayString(TEXT_DIR_LINE_FILENAME_BARE, file->ff_name);
      rv = incline();
    }
    else
    {
      char buffer[sizeof(long) * 4 + 2], *ext = "";

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
        convert(file->ff_fsize, 0, buffer);
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
        outc(' ');
		outs(p);
		free(p);
		p = nls_maketime(NLS_MAKE_SHORT_AMPM, hour, minute, -1, 0);
		if(!p) {
			error_out_of_memory();
			return E_NoMem;
		}
        outc(' ');
		outs(p);
		free(p);
		#ifdef FEATURE_LONG_FILENAMES
            if( dispLFN )
				printLFNname(file->ff_name, ext);
		#endif
		#ifdef FEATURE_DESCRIPT_ION
			if (descriptionExists)
				showDescription(file->ff_name, ext);
		#endif
        outc('\n');
	 }

      rv = incline();
   }
   return cbreak? E_CBreak: rv;
}

/* return -1,0,1 where -1 if p1 is before p2, 0 if same, 1 if p2 is before p1 */
static int orderFunction(const void *p1, const void *p2)
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
  
  /* if criteria (size, date, etc) compare equal then return results lexically by filename */
  if (rv == 0)
	rv = strcmp(f1.ff_name,f2.ff_name);

  return rv;
}    	


static int flushOrder(struct ffblk _seg *orderArray,
  		int *orderIndex,
  		int  orderCount,
		struct currDir *cDir)
{
  int i;
  struct ffblk file;
  
  for (i = 0; i < orderCount; i++)
  	orderIndex[i] = i;
  	
  qsort(orderIndex, orderCount, sizeof(orderIndex[0]), orderFunction);

  for (i = 0; i < orderCount; i++) {
  	int rv;
    _fmemcpy(&file, &orderArray[orderIndex[i]], sizeof(file));
    if((rv = DisplaySingleDirEntry(&file, cDir)) != E_None)
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
  , bignum *bcnt
  )
{
  struct ffblk file;
  bignum bytecount = {0};
  unsigned long filecount = 0;
  unsigned long dircount = 0;
  int rv = E_None;
  struct currDir cDir = {0};
  
#define MAX_ORDER (0xffff / sizeof(struct ffblk))
  int *orderIndex = NULL;
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
#ifdef FARDATA
		/* use last-fit allocation to work well with large model */
		orderArray = MK_SEG_PTR(void, DOSalloc(0x1000,2));
#else
		orderArray = MK_SEG_PTR(void, DOSalloc(0x1000,0));
#endif
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
#ifdef FEATURE_DESCRIPT_ION
  strcpy(&path[pathlen], "DESCRIPT.ION");
  descriptionExists = ((fDescription = dos_open(path, O_RDONLY)) >= 0);
#endif
  strcpy(&path[pathlen], pattern);

  if (sfnfindfirst(path, &file, attrMay) == 0) {
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
        outc('\n');
        rv = incline();
    }
   }
  }

    if (cbreak)
      rv = E_CBreak;
    else if (rv == E_None && file.ff_attrib != FA_DEVICE) {
		if(file.ff_attrib & FA_DIREC) {
			dircount++;
		} else {
			bignum tmp;
			filecount++;
			tmp.low = (unsigned long)file.ff_fsize % 1000000000ul;
			tmp.billions = (unsigned long)file.ff_fsize / 1000000000ul;
			bignum_add(&bytecount, &tmp);
		}
		if(optO) {  
			_fmemcpy(&orderArray[orderCount], &file, sizeof(file));
			orderCount++;
			if(orderCount >= MAX_ORDER) {
				rv = flushOrder(orderArray,orderIndex,orderCount,&cDir);
				orderCount = 0;
			}
		} else
			rv = DisplaySingleDirEntry(&file, &cDir);
    }
  }
  while (rv == E_None && sfnfindnext(&file) == 0);
  }

  #ifdef FEATURE_DESCRIPT_ION
    if (descriptionExists) dos_close(fDescription);
  #endif


  if(optO) {
    if(rv == E_None)
		rv = flushOrder(orderArray,orderIndex,orderCount,&cDir);
    free(orderIndex);
    DOSfree(FP_SEG(orderArray));
  }

  if (rv == E_None && optW && (cDir.linecount != 0)) {
    outc('\n');
    rv = incline();
  }

	if(rv == E_None) {
		if(filecount || dircount)
			rv = print_summary(filecount, &bytecount);
		else if(!optS) {
			error_file_not_found();
			rv = E_Other;
		}
	}

  if(rv == E_None       /* no error */
   && optS) {            /* do recursively */
      /* already set for optB && optS before do {} while above 
		  path[pathlen - 1] = '\\';		*/
      strcpy(&path[pathlen], "*.*");
      	/* Import attributes S & H from "maybe" */
      if (sfnfindfirst(path, &file, attrMay | FA_DIREC) == 0) do {
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
      } while (rv == E_None && sfnfindnext(&file) == 0);
  }

    *dcnt += dircount;
    *fcnt += filecount;
    bignum_add(bcnt, &bytecount);

  return rv;
}


static int dir_print_body(char *arg, unsigned long *dircount)
{	int rv;
	unsigned long filecount;
	bignum bytecount = {0};
	char *pattern, *cachedPattern;
	char *p;
#if 0
    char altarg[ MAXPATH ];

    if( ( p = strrchr( arg, '\\' ) ) == NULL && arg[ 1 ] ) {
        if( *arg = '.' ) {
            sprintf( altarg, "*%s", arg );
        } else if( strchr( arg, '.' ) == NULL ) {
            sprintf( altarg, "%s.*", arg );
        }
    } else if( p[ 1 ] ) {
        if( *arg = '.' ) {
        }
    }
#endif

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

	filecount = 0;

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
		rv = print_total(filecount, &bytecount);
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
int cmd_dir (char * rest) {
  char **argv;
  char *p;
  int argc, opts;
  int rv;                       /* return value */
  unsigned long dircount;
  int appState;					/* DOS/APPEND state */

  /* initialize options */
  attrMask = attrMatch = optO = longyear = optS = optP = optW = optB = optL = 0;
#ifdef FEATURE_LONG_FILENAMES
  dispLFN = 0;
#endif
  attrMay = ATTR_DEFAULT;

  /* read the parameters from env */
  if ((argv = scanCmdline(p = getEnv("DIRCMD"), opt_dir, 0, &argc, &opts))
   == 0) {
    free(p);
    return 1;
  }
  free(p);
  freep(argv);    /* ignore any parameter from env var */

  line = 0;
  /* read the parameters */
  if ((argv = scanCmdline(rest, opt_dir, 0, &argc, &opts)) == 0)
    return 1;

  /* critEnableRepeatCheck(); */
  appState = appendDisable();
  dircount = 0;
  if(argc)
    for(opts = 0
     ; opts < argc && (rv = dir_print_body(argv[opts], &dircount)) == 0
     ; ++opts)
      ;
  else
    rv = dir_print_body(".", &dircount);
  /* critEndRepCheck(); */

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
int cmd_vol (char * rest) {
  int drive;

  assert(rest);

  /* save the current directory info */
      line = 0;
      optB = 0;

  if (rest && *rest)
  {
    /* trim whitespace after drive indicator, else extra spaces cause syntax error */
    rtrimcl(rest);

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
