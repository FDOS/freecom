/*	$Id$

 * Grab the filename of COMMAND.COM
 *
 *  If warn != 0, warnings can be issued; otherwise this functions
 *  is silent.

	This file bases on INIT.C of FreeCOM v0.81 beta 1.

	0 -> is valid
	1 -> no such file
	2 -> is a device
	3 -> not valid
	4 -> out of memory / syntax error ... .

	$Log$
	Revision 1.6  2004/06/29 14:14:55  skaus
	fix: help screen of internal commands causes "Unknown command error" {Bernd Blaauw}

	Revision 1.5  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files
	
	Revision 1.4  2004/02/01 13:24:22  skaus
	bugfix: misidentifying unspecific failures from within SUPPL
	
	Revision 1.3  2003/12/09 20:25:34  skaus
	bugfix: INIT: FreeCOM-path and TTY can be in any order
	
	Revision 1.2  2002/11/05 19:29:21  skaus
	bugfix: FreeCOM should accept relative path as argv[0]
	
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
#include <dos.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <dfn.h>

#include "../include/command.h"
#include "../err_fcts.h"
#include "../include/misc.h"

int grabComFilename(const int warn, const char far * const fnam)
{
  char *buf;
  size_t len;
  int rc;

  dprintf( ("[INIT: grabComFilename(%s)]\n", fnam) );
  if(!fnam)
  	return 4;

  /* Copy the filename into the local heap */
  len = _fstrlen(fnam);
  if(len >= INT_MAX || len < 1) {
    /* no filename specified */
    if(warn)
      error_syntax(0);
    return 4;
  }

  if((buf = malloc(len + 1)) == 0) {
    if(warn) error_out_of_memory();
    return 4;
  }
  _fmemcpy((char far*)buf, fnam, len);
  buf[len] = '\0';

    if (buf[1] != ':' || buf[2] != '\\')
    { char *p;

        /* expand the string for the user */
      p = abspath(buf, warn);
      free(buf);
      if((buf = p) == 0)
		  return 4;
      if(warn)
          error_init_fully_qualified(buf);

    	len = strlen(buf);
    }

    while(buf[len - 1] == '\\')
    	--len;
    buf[len] = 0;

    if(dfnstat(buf) & DFN_DIRECTORY) {
      /* The user specified a directory, try if we can find the
        COMMAND.COM with the standard name in there */
      char *p;

      if((p = realloc(buf, len + sizeof(COM_NAME) + 1)) == 0) {
        if(warn) error_out_of_memory();
        free(buf);
        return 4;
      }
      buf = p;
      strcpy(&buf[len], "\\" COM_NAME);
    }


	if(0 != (rc = validResFile(buf))) {
		if(warn) switch(rc) {
		default:
	#ifdef NDEBUG
			assert(0);
	#endif
		case 1: error_open_file(buf); break;
		case 2: error_fcom_is_device(buf); break;
		case 3: error_fcom_invalid(buf); break;
		}

		free(buf);
		return rc;
	}

  free(ComPath);    /* Save the found file */
  ComPath = buf;
	dprintf(("[INIT: new resource file name: %s]\n", ComPath));

  isSwapFile = 0;
  buf = dfnfilename(ComPath);
  assert(buf);
  if((buf = strchr(buf, '.')) != 0
   && stricmp(buf, ".swp") == 0) {
       dprintf(("[INIT: VSpawn file found: %s]\n", ComPath));
       memcpy(++buf, "COM", 3);
       isSwapFile = buf - ComPath;
  }

  return 0;
}
