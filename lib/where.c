/*	$Id$

	Search for an external program and return it in a static buffer,
	which is overwritten the next time this function is called.

	This file bases on OPENF.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.12  2006/08/23 06:45:37  blairdude
	Updated the PATH parsing routine (Arkady)

	Revision 1.8  2006/06/27 07:09:26  blairdude
	find_which now uses simpler functions to parse %PATH% to find executables.
	
	Revision 1.7  2006/06/26 18:36:36  blairdude
	FreeCOM can now execute long filenamed executable files.
	
	Revision 1.6  2005/12/10 10:09:43  perditionc
	based on patches from Blair Campbell, additional LFN support (slim print,
	add initial cd,rd,md support, make compile time optional), remove some
	compiler warnings, and prevent extra linebreak for compatibility
	
	Revision 1.5  2004/09/13 18:59:40  skaus
	add: CRITER: Repeat check autofail magic {Tom Ehlert/Eric Auer}
	
	Revision 1.4  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files
	
	Revision 1.3  2004/02/01 13:24:22  skaus
	bugfix: misidentifying unspecific failures from within SUPPL
	
	Revision 1.2  2002/11/05 19:27:37  skaus
	bugfix: FreeCOM is to pass the fully-qualified path to DOS-4B-00
	
	Revision 1.1  2001/04/12 00:33:53  skaus
	chg: new structure
	chg: If DEBUG enabled, no available commands are displayed on startup
	fix: PTCHSIZE also patches min extra size to force to have this amount
	   of memory available on start
	bugfix: CALL doesn't reset options
	add: PTCHSIZE to patch heap size
	add: VSPAWN, /SWAP switch, .SWP resource handling
	bugfix: COMMAND.COM A:\
	bugfix: CALL: if swapOnExec == ERROR, no change of swapOnExec allowed
	add: command MEMORY
	bugfix: runExtension(): destroys command[-2]
	add: clean.bat
	add: localized CRITER strings
	chg: use LNG files for hard-coded strings (hangForEver(), init.c)
		via STRINGS.LIB
	add: DEL.C, COPY.C, CBREAK.C: STRINGS-based prompts
	add: fixstrs.c: prompts & symbolic keys
	add: fixstrs.c: backslash escape sequences
	add: version IDs to DEFAULT.LNG and validation to FIXSTRS.C
	chg: splitted code apart into LIB\*.c and CMD\*.c
	bugfix: IF is now using error system & STRINGS to report errors
	add: CALL: /N
	
 */

#include "../config.h"

#include <stdlib.h>	/* free */
#include <string.h>	/* strlen, strcpy, memcpy */

#include "suppl.h"
#include "dfn.h"	/* dfnstat, DFN_FILE, dfnfullpath */

#include "../include/lfnfuncs.h" /* getshortfilename */
#include "../include/misc.h"	/* getEnv, display_errno_fnam_error */

/*
- Name without suffix(es) ("cmd") always searched with implicit suffixes
  ("cmd.com", "cmd.exe" and "cmd.bat").
- Name with explicit suffix(es) ("cmd.", "cmd.exe", "cmd.exe.bat") searched
  only with these suffix(es).
- Found directory names are skiped.
- PATH variable interated only for name without path ("cmd") or with
  relative path ("dir\cmd"); name with drive letter ("d:cmd") or absolute
  path ("\cmd", "\dir\cmd") searched only at this path.

   Returned pointer references heap, and this memory area
   will be freed on subsequent calls.
*/

char *find_which (const char *const name) {
  static char *pbuf = NULL;		/* previously returned pointer	*/
  char *envptr, *path;			/* content of PATH env variable	*/
  char buf [MAXPATH + 4];		/* buffer for PATH entry + name	*/
  char *pname, *p;
  size_t len;

  free (pbuf), pbuf = NULL;

  /* copy name at end of buffer */
  len = strlen (name);
  if (len > sizeof buf - 5)			/* not enough space?	*/
    return pbuf;
  p = buf + sizeof buf - 5,
  pname = strcpy (p - len, name);

  /* check for explicit suffix in name */
  /*if (p > pname)*/
  for (;;) {
    --p;
    if (*p == '.') break;			/* explicit suffix?	*/
    if (p <= pname ||				/* start of name?	*/
        *p == '\\' || *p == '/' || *p == ':') {	/* path separator?	*/
      buf [sizeof buf - 5] = '.';		/* use implicit suffixes */
      break;
    }
  }

  /* check for drive letter or absolute path */
  p = pname, envptr = NULL;
  if (p [1] != ':' && *p != '\\' && *p != '/')
    envptr = getEnv ("PATH");
  path = envptr;

  /* check current directory, then all entries in PATH */
  for (;;) {
    if (buf [sizeof buf - 5] == '.') {
      strcpy (buf + sizeof buf - 4, "COM"); if (dfnstat (p) & DFN_FILE) break;
      strcpy (buf + sizeof buf - 4, "EXE"); if (dfnstat (p) & DFN_FILE) break;
      strcpy (buf + sizeof buf - 4, "BAT");
    }
					    if (dfnstat (p) & DFN_FILE) break;

    /* find next PATH entry, which fit in buf */
    if (envptr == NULL || pname == buf) { free(path); return pbuf; }
    do {
      while (*envptr == ';') ++envptr;		/* skip sequence of ';'	*/
      if (*envptr == '\0') return pbuf;		/* no more PATH entries? */

      /* count PATH entry len */
      len = 0;
      do ++len, ++envptr;
      while (*envptr && *envptr != ';');

      /* add separator, if necessary, between name and path */
      p = pname;
      if (envptr [-1] != '\\' && envptr [-1] != '/' && envptr [-1] != ':')
        --p, *p = '\\';
    } while (len > p - buf);

    /* add path before name */
    p = memcpy (p - len, envptr - len, len);
  }
  free(path);

  /* file found, now convert its path to full-qualified absolute one */
#ifdef FEATURE_LONG_FILENAMES
  if ((pbuf = dfnfullpath (getshortfilename (p))) == NULL)
    display_errno_fnam_error (getshortfilename (p));
#else
  if ((pbuf = dfnfullpath (p)) == NULL)
    display_errno_fnam_error (p);
#endif
  return pbuf;
}
