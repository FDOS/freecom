/* $Id$
   $Locker$	$Name$	$State$

	Error codes used by most functions returning (int) error
	codes within SUPPL

   $Log$
   Revision 1.1  2000/07/09 22:19:22  skaus
   + Support for international strings without recompiling
   + Useage of TC++1

   Revision 1.1  2000/03/31 09:09:32  ska
   Initial revision

*/

#ifndef __ERRCODES_H
#define __ERRCODES_H

#define ESUPPL_NONE		 0		/* no error */
#define ESUPPL_OK		 0		/* no error */
#define ESUPPL_ZERO		 0		/* no error */
#define ESUPPL_MEM		 1		/* out of memory */
#define ESUPPL_NOMEM	ESUPPL_MEM
								/* env: environment segment too small */
#define ESUPPL_MISSINFO	 2		/* missing information */
#define ESUPPL_FPATH	30		/* ill-formed filename path */
#define ESUPPL_FACCESS	31		/* file access error */
#define ESUPPL_INIT		40		/* Not initialized */

#define ESUPPL_INVAL	50		/* Invalid argument, argument out of range */
#define ESUPPL_INVENV	51		/* Invalid environment */
#define ESUPPL_RANGE	52		/* Result too large */
								/* env: returned string too large for buffer */
#define ESUPPL_NOENV	53		/* No environment at all */
#define ESUPPL_NOENT	55		/* No entry / no such file or directory */
								/* env: no such variable / string */

#endif
