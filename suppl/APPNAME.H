/* $Id$
   $Locker$	$Name$	$State$

	Declarations for the application name/path etc. functions.
	For details refer to DOC\DETAILS.TEX

   $Log$
   Revision 1.1  2000/07/09 22:19:22  skaus
   + Support for international strings without recompiling
   + Useage of TC++1

   Revision 1.3  2000/03/31 09:09:31  ska
   add: DBG_CLOSELOG, suppl_l_openmode
   add: SYSLOG_ALWAYS_FLUSHLOG
   add: fcommit(), Fcommit(), Fflush(), commit()
   add: suppl_log_flush(), DBG_FLUSHLOG, DBG_ALWAYS_FLUSHLOG
   fix: dfnsearch(): DBG_ARGUMENT()
   chg: F[gs]etpos() -> true replacements of f[gs]etpos(); removed
   	Fp[gs]etpos(); added FF[gs]etpos() using a non-pointer argument
   bugfix: secure string functions: memory functions don't accept length == 0
   add: MKSTRFCT.PL: generate DOC\SSTR.FCT
   fix: dfnsplit(): a pathname without path components returns the root
   	directory as path portion
   add: dfnsplit(): debug output of return values of found drive/path/name/ext
   fix: dfnsqueeze(): DBG_ENTER() contains wrong names
   fix: dfnsplit(): chkHeap in drive spec detection routine breaks if/else
   chg: moved NLS-depended stuff from DFN.H into NLS_F.H
   add: integer SUPPL error codes -- all functions returning (int) error codes
   	return unique codes used throughout all SUPPL, see ERRCODES.H

   Revision 1.2  1999/01/18 05:32:29  ska
   add: INI file writing functions
   fix: dfnstat() uses wrong register for result
   add: dfnfreplace(), dfnbakname()

   Revision 1.1  1999/01/13 03:26:03  ska
   Initial revision

*/

#ifndef __APPNAME_H
#define __APPNAME_H

#ifndef __PORTABLE_H
#include <portable.h>
#endif
#ifndef __DFN_H
#include "dfn.h"
#endif

char *appName(void);
char *appPath(void);
char *appFile(void);
char *appNameEx(void);
char *appPathEx(void);
char *appFileEx(void);
int appInit(const char * const fnam);
int appInitEx(const char * const fnam);
int appInitEnv(void);
int appInitEnvEx(void);

#endif
