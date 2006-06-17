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

ob(ject): env_strinsert
su(bsystem): env
ty(pe): 
sy(nopsis): 
sh(ort description): Insert a string before an existing one
he(ader files): 
lo(ng description): Inserts an environment string before the specified one.
	The index of all following strings increments by one.\par If
	\tok{\para{index} == env_strings()}, a new string is appended.
pr(erequistes): 
va(lue): integer SUPPL error code
re(lated to): env_string
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers):

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

int env_strinsert(word segm, const char * const buf, const int index)
{	word ofs, len;

	DBG_ENTER("env_strinsert", Suppl_env)

	chkMem

	if(!buf) DBG_RETURN_I( ESUPPL_INVAL)	/* no string to insert */

	DBG_ARGUMENTS( ("env=%u, str=\"%s\", idx=%d", segm, buf, index) )

	unless_segm(segm)
		DBG_RETURN_I( ESUPPL_NOENV)		/* no environment at all */
	
	DBG_ARGUMENTS( ("effective env=%u", segm) )

	if(env_freeCount(segm) <= (len = strlen(buf) + 1))
		DBG_RETURN_I( ESUPPL_NOMEM)			/* not enough room */

	if(!(ofs = env_string(segm, index)))
		DBG_RETURN_I( ESUPPL_NOENT)		/* string number too high */

	/* Move the following strings out of the way */
	_fmemmove(MK_FP(segm, ofs + len), MK_FP(segm, ofs)
	 , env_firstFree(segm) - ofs);
	chkMem

	/* Now, there is enough room at segm:ofs to put string there */
	_fmemcpy(MK_FP(segm, ofs), TO_FP(buf), len);
	chkMem

	/* Increment string counter */
	env_strcounter(segm, 1);

	DBG_RETURN_I( ESUPPL_OK)
}
