/*	$Id$

 *  Set attributes of an opened handle
 *
 *  Return:
 *    0: on succes
 *    else: DOS error code

	$Log$
	Revision 1.4  2006/06/11 02:47:05  blairdude
	Optimized FreeCOM for size, fixed LFN bugs, and started an int 2e handler (which safely fails at the moment)

	Revision 1.3  2005/12/16 12:03:39  perditionc
	use proper subfunction to set attributes (from Eric)
	
	Revision 1.2  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files
	
	Revision 1.1  2002/04/08 16:02:22  skaus
	bugfix: CTTY: added to try to set the device flags
	
 */

#include <dos.h>
#include <portable.h>
#include "../config.h"

int fdsetattr(const int fd, const int attr)
{
  IREGS r;

  r.r_ax = 0x4401;              /* Get handle information */
  r.r_bx = fd;
  r.r_dx = attr;
  intrpt(0x21, &r);
  return ( r.r_flags & 1 )         /* call failed */
  ? r.r_ax          /* error code */
  : 0;            /* OK */
}
