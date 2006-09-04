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

ob(ject): dfntruename
su(bsystem): dfn
ty(pe): H
sh(ort description): Return the truename of a given filename
he(ader files): 
lo(ng description): Returns the truename of a given filename as returned
	by the DOS API function 0x60. The filename must not exceed the length
	of \tok{DFN_FILENAME_BUFFER_LENGTH} bytes.\par
	The possibly only useful function in which \tok{dfntruename()}
	should be used is to determine if two files are actually the same
	physical file although they have different logical filenames, e.g. assume
	the file \tok{C:\FREEDOS\SUBST.EXE} exists and one has created the
	logical drive letter \tok{Z:} by the following command:
	\tok{SUBST Z: C:\FREEDOS}. One could access the same \em{physical}
	file via two different logical path specifications:
	\enum \tok{Z:\SUBST.EXE}, and
	\enum \tok{C:\FREEDOS\SUBST.EXE}.
	\endlist
	\tok{dfntruename()} applied to both path specifications would
	return the only physical access path used by the DOS kernel.\par
	\em{But beware!} The truename must not be used as a function to fully-qualify
	a filename. There are at least two cases when that fails:
		\enum		\tok{JOIN A: C:\LW-A}\newline
			Now the call
			\tok{dfntruename("C:/lw-a/file")} results into \tok{"A:\\FILE"},
			but drive \tok{A:} is not accessable at all!
		\enum Networked drives, e.g. CD-ROM drives and LAN drives, does not
		follow the standard naming. Most of them (if not all) follow
		the UNC standard. For example:\newline
			Assume drive \tok{F:} is a Novell Netware drive created by:\newline
				\tok{MAP ROOT F:=SERVER/VOLUME:PATH1/PATH2/PATH3}\newline
			the call \tok{dfntruename("f:\\file")} would return
				\tok{"\\SERVER\\VOLUME\\PATH1\\PATH2\\PATH3\\FILE"}.\par
			Windows 9x/NT can handle UNC filenames, but this results in
			much slower access time, because Windows 95 globs the filename
			through the "network environment" driver each time such name
			is used.
pr(erequistes): 
va(lue): NULL: on failure (OS error code)\item else: pointer to dynamically allocated
	filename
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): The returned filename \em{cannot} be used to generate a
	fully-qualified pathname.\par
	The only useful thing to do with the result is to test, if two filenames
	point to the same physical file. One can think of this function as it
	would return a string generated of the device and i-node number of the
	file in Unix (symbolic links resolved).
bu(gs): 
fi(le): dfntruen.c

*/

#include "../../config.h"
#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <stdlib.h>
#endif
#include "dfn.loc"
#include "dynstr.h"
#include "suppl.h"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *dfntruename(const char * const fnam)
{	char *h;
	struct REGPACK r;

	DBG_ENTER("dfntruename", Suppl_dfn)

	assert(fnam);

	DBG_ARGUMENTS( ("fnam=\"%s\"", fnam) )

	chkHeap
	if((h = eno_malloc(DFN_FILENAME_BUFFER_LENGTH)) != 0) {
#ifdef FEATURE_LONG_FILENAMES
        r.r_ax = 0x7160;
        r.r_cx = 0x8002;
#else
		r.r_ax = 0x6000;
#endif
        r.r_ds = FP_SEG(fnam);
		r.r_si = FP_OFF(fnam);
		r.r_es = FP_SEG(h);
		r.r_di = FP_OFF(h);
		chkHeap
        intr( 0x21, &r );
#ifdef FEATURE_LONG_FILENAMES
        if( ( r.r_flags & 1 ) || r.r_ax == 0x7100 ) {
            r.r_ax = 0x6000;
            intr( 0x21, &r );
#endif
		if(( r.r_flags & 1 ) ? r.r_ax : 0) {		/* failed */
			eno_setOSerror( r.r_ax);
			free(h);
			DBG_RETURN_S( 0)
		}
#ifdef FEATURE_LONG_FILENAMES
        }
#endif
	}

	DBG_RETURN_BS( StrTrim(h))
}
