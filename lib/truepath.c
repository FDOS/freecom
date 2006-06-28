/*	$Id$

 *	Make the given file spec an absolute path/file name.
 *	Returns in a dynamically allocated buffer (free'ed by the caller)
 *	on error: Displays "out of memory" (or appropriate)

	$Log$
	Revision 1.3  2006/06/28 06:29:11  blairdude
	TRUENAME now supports long filenames

	Revision 1.2  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files
	
	Revision 1.1  2004/02/01 13:24:22  skaus
	bugfix: misidentifying unspecific failures from within SUPPL
	
 */

#include "../config.h"

#include <assert.h>
#include <errno.h>

#include <dfn.h>

#include "../include/lfnfuncs.h"
#include "../err_fcts.h"
#include "../include/command.h"
#include "../include/misc.h"

char *truepath(const char * const fnam)
{	char *h;

	assert(fnam);

#ifdef FEATURE_LONG_FILENAMES
	if((h = dfntruename(getshortfilename(fnam))) != 0)
#else
    if((h = dfntruename(fnam)) != 0)
#endif
		return h;

	display_errno_fnam_error(fnam);
	return 0;
}
