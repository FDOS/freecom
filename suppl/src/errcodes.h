/*
    This file is part of SUPPL - the supplemental library for DOS
    Copyright (C) 1996-2000 Steffen Kaiser

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/* $Id$
   $Locker$	$Name$	$State$

	Error codes used by most functions returning (int) error
	codes within SUPPL

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
