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
/* $RCSfile$
   $Locker$	$Name$	$State$

ob(ject): dfnstat
su(bsystem): dfn
ty(pe): 
sh(ort description): Return the attributes of a given file
he(ader files): 
lo(ng description): Returns the attributes of a given file. If non-zero, the
	return value is a OR'ed combination of the following flags:
	\item tok{DFN_FILE}: file is a regular file
	\item tok{DFN_LFN}: LFN entry
	\item tok{DFN_DIRECTORY}: file is a directory
	\item tok{DFN_LABEL}: file is a value label
	\item tok{DFN_READ}: read permissions granted
	\item tok{DFN_WRITE}: write permissions granted (may invalid for dirs)
	\item tok{DFN_HIDDEN}: not for normal display to user
	\item tok{DFN_SYSTEM}: file important for system, unmoveable
 	\nolist 
	Wildcards are \em{not} expanded, if they are present in the filename.
pr(erequistes): 
va(lue): 0: on failure (file/path not found) (OS error code)\item else: file attribute as
	described above
re(lated to): dfnwrdir
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dfnstat.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <ctype.h>
#endif

#include "suppl.h"
#include "dfn.loc"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int dfnstat(const char * const fnam)
{	struct REGPACK r;
	int attr;

	DBG_ENTER("dfnstat", Suppl_dfn)
	DBG_ARGUMENTS( ("fnam=\"%s\"", fnam) )

	assert(fnam);

	/* Probe for root directory */
	if(*fnam && fnam[1] == ':'
	 && (!fnam[2] || (fnam[2] == '\\' && !fnam[3]))) {
	 	/* Root spec or drive letter only */
	 	/* Probe if the drive is ready */
	 	r.r_ax = 0x4409;
	 	r.r_bx = toupper(*fnam) - 'A' + 1;
        intr( 0x21, &r );
        if(( r.r_flags & 1 ) ? r.r_ax : 0) {
			eno_setOSerror(r.r_ax);
			DBG_RETURN_I( 0)
		}
		DBG_RETURN_X( DFN_DIRECTORY)
	}

	r.r_ax = 0x4300;		/* get file attributes */
	r.r_ds = FP_SEG(fnam);
	r.r_dx = FP_OFF(fnam);
    intr( 0x21, &r );
	if( ( r.r_flags & 1 ) ? r.r_ax : 0) {
		eno_setOSerror(r.r_ax);
		DBG_RETURN_I( 0)			/* call failed */
	}

	if(((attr = r.r_cx) & 0xf) == 0xf)
		attr = DFN_LFN;
	else if(!((attr ^= DFN_WRITE) & (DFN_DIRECTORY | DFN_LABEL)))
		attr |= DFN_FILE;

	DBG_RETURN_X( attr)
}
