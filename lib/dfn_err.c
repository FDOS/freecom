/*	$Id$

	Display a nice errno-depend error message for
	dfn*() functions.

	$Log$
	Revision 1.2  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files

	Revision 1.1  2004/02/01 13:24:22  skaus
	bugfix: misidentifying unspecific failures from within SUPPL
	
 */

#include "../config.h"

#include <assert.h>
#include <errno.h>

#include "../err_fcts.h"
#include "../include/misc.h"

void display_errno_fnam_error(const char * const fnam)
{
	assert(fnam);

	switch(errno) {
	case ERANGE:	dprintf( ("[FATAL: dfnpath() buffer too small]\n") );
	case ENOMEM:	error_out_of_memory(); break;
	default:		error_sfile_not_found(fnam);
	}
}
