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

ob(ject): env_strcpy
su(bsystem): env
ty(pe): 
sy(nopsis): 
sh(ort description): Copy an environment string into a buffer
he(ader files): 
lo(ng description): Copies the \para{index}'th environment string into
	the specified buffer.
pr(erequistes): \tok{\para{buffer} != NULL && \para{len} > 0}
va(lue): integer SUPPL error code
re(lated to): env_string
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <dos.h>
#endif
#include "errcodes.h"
#include "environ.loc"
#include "fmemory.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int env_strcpy(word segm, char * const buf, const int len, const int index)
{	word ofs;

	DBG_ENTER("env_strcpy", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, idx=%d", segm, index) )

	assert(buf);
	assert(len > 0);

	chkMem
	
	unless_segm(segm)
		DBG_RETURN_I( ESUPPL_NOENV)			/* no environment at all */

	DBG_ARGUMENTS( ("effective env=%u", segm) )

	if(!(ofs = env_string(segm, index))
	 || ofs == env_firstFree(segm))
		DBG_RETURN_I( ESUPPL_NOENT)			/* string number too high */

	_fmemcpy(TO_FP(buf), MK_FP(segm, ofs), len);
	buf[len - 1] = '\0';	/* Make sure that the string is terminated */
	chkHeap

	DBG_RETURN_I( ((strlen(buf) < len - 1)	/* buffer not filled completely */
	 || !peekb(segm, ofs + len - 1))		/* variable exact == len */
	  ? ESUPPL_OK							/* OK */
	  : ESUPPL_RANGE)						/* buffer too small */
}
