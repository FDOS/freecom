/*	$Id$

	Truncate an opened file at the current position of the file
	pointer by file descriptor.
	Return: 0 -> OK
			!= 0 -> error

	$Log$
	Revision 1.2  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files

	Revision 1.1  2003/05/12 16:52:41  skaus
	add: copy(): Speed-Up by pre-creating the file {Tom Ehlert}
	
 */

#include "../config.h"

#include <assert.h>

#include <suppl.h>

#include "../include/misc.h"

int truncate(int fd)
{	IREGS r;

	r.r_ax = 0x4000;	/* Write to file descriptor */
	r.r_bx = fd;
	r.r_cx = 0;			/* Indicates "truncate" */
	return invokeDOS(&r);
}
