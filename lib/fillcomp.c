/*	$id$
	$Locker$	$Name$	$State$

	Substitute wildcards in filenames

	fillFnam():
	Substitutes the wildcards of the last filename component.

	fillComp():
	replaces the wildcards ? and * of a filename pattern with characters
	from a source filename (useful in COPY a*.* b?1.*).
	If a question mark appears bejond the end of the file name, it
	is silently ignored, e.g. in above example if copy'ing A.TXT)

	This file bases on COPY.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.2  2003/04/08 13:37:57  skaus
	chg: wilcard matching: COPY/REN *.TXT ???b.*: If length of source filename
		is less than 3, the superflous question marks are ignored, as in MS
		COMMAND v6.22 COPY, but INCOMPATIBLE to same version's REN, which
		embeds spaces

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

#include <assert.h>
#include <dir.h>
#include <stdlib.h>
#include <string.h>

#include <dfn.h>

#include "../include/command.h"

static void fillComp(char * const dst
 , const char *pattern
 , const char *src
 , int len)
{ char *s;

  assert(len);
  assert(dst);

  if(!pattern)
    pattern = "";
  if(!src)
    src = "";

  s = dst;
  while(--len) {
    switch(*s = *pattern++) {
    case '\0':
      goto ende;
    case '?':
    	if(!*src)
    		continue;
/*      if(*src) do not keep ? bejond end-of-filename */
        *s = *src;
    default:
      ++s;
      break;
    case '*':
      memcpy(s, src, len);
      s[len] = '\0';
      return;
    }
    if(*src)
      ++src;
  }
ende:
#if 0		/* don't keep ? bejond end-of-filename */
  /* The pattern may have less characters than the source, because
  	trailing '?'s do match "nothing" in DOS. */
  while(--s >= dst && *s == '?');
  s[1] = '\0';
#else
  *s = '\0';
  assert(strchr(dst, '?') == 0);
#endif
}

char *fillFnam(const char * const pattern
 , const char * const fnam)
{ char *dr, *pa, *fn, *ex;
  char *pfn, *pex;
  char dfn[MAXFILE], dex[MAXEXT];
  char *p;

  assert(fnam);
  assert(pattern);

  if(strchr(pattern, '?') == 0 && strchr(pattern, '*') == 0)
    return strdup(pattern);

  if(!dfnsplit(pattern, &dr, &pa, &fn, &ex))
    return 0;

  if(!dfnsplit(fnam, 0, 0, &pfn, &pex)) {
    free(fn);
    free(ex);
    free(dr);
    free(pa);
    return 0;
  }

  fillComp(dfn, fn, pfn, MAXFILE);
  fillComp(dex, ex, pex, MAXEXT);

  p = dfnmerge(0, dr, pa, dfn, dex);

  free(pfn);
  free(pex);
  free(dr);
  free(pa);
  free(fn);
  free(ex);

  return p;
}
