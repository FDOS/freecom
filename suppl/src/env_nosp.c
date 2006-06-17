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

ob(ject): env_noSpace
su(bsystem): env
ty(pe): L
sh(ort description): Test if the environment can hold a new variable
lo(ng description): Tests if a new variable can be added to the environment,
	if there are still enough unused bytes.\par
	\para{ignore} specifies an amount of bytes that are unused in addition
	to the already free bytes within the environment currently.\par
	Both \para{var} and \para{value} may be \tok{NULL} and there won't be
	performed any tests on their contents.
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): \endlist integer SUPPL error code\newline
	\item \tok{ESUPPL_OK}: success
	\item \tok{ESUPPL_NOMEM}: not enough unused bytes
	\item \tok{ESUPPL_NOENV}: no environment at all
fi(le): env_nosp.c

*/

#include "initsupl.loc"

#include <portable.h>
#include "errcodes.h"
#include "environ.loc"
#include "sstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int env_noSpace(word segm, const char * const var, const char * const value
	, const word ign)
{	int needSpace;

	DBG_ENTER("env_noSpace", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, var=\"%s\", value=\"%s\", ignore=%u", segm, var, value, ign) )

	chkMem
	
	unless_segm(segm)
		DBG_RETURN_I( ESUPPL_NOENV)

	DBG_ARGUMENTS( ("effective env=%u", segm) )

	DBG_RETURN_BI(
			/* required space fits in the already ignored space */
		((needSpace = Strlen(var) + Strlen(value) + 2) < ign
			/* required space fits in still unused portion of env */
		 || env_freeCount(segm) > needSpace - ign)
		 	? ESUPPL_OK
		 	: ESUPPL_NOMEM
	)
}
