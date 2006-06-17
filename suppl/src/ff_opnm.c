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

	void F_openmode(char *mode, int *arg)

	Decode a string-mode into individual modes

ob(ject): F_openmode
su(bsystem): supplio
ty(pe): 
sy(nopsis): 
sh(ort description): Decode a 'mode' string into open mode flags
he(ader files): 
lo(ng description): Decodes a Micro-C 'mode' string into the
	open mode flags used by \subsys{supplio}. Micro-C uses different
	modes than ANSI C.
pr(erequistes): 
va(lue): none
re(lated to): 
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): Micro-C only

*/

#include "initsupl.loc"

#ifdef _MICROC_
#include "supplio.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void F_openmode(char *mode, FLAG *arg)
{
	DBG_ENTER("F_openmode", Suppl_supplio)

	assert(mode);

	DBG_ARGUMENTS( ("mode=\"%s\"", mode) )

	assert(arg);

	/* Note: The order must match the one of FF_OPENMODE_ARGS! */
#define r arg[0]
#define w arg[1]
#define a arg[2]
#define u arg[3]
#define b arg[4]

	/* decompose the mode-string */
	r = w = a = u = b = 0;
	while(*mode) switch(*mode++) {
	case 'r': r = 1; break;
	case 'w': w = 1; break;
	case 'a': a = 1; break;
	case '+': u = 1; break;
	case 'b': b = 1; break;
	case 't': b = 0; break;
	}
							/* "a" is reverted to "w" with the exception
								that "a" probes for the existance of
								the file first */
	if(r & (w |= a))		/* "rw" is "r+" or, if failed,  "w+" */
		u = 1;

	DBG_RETURN_VALUES( ("flags: rd=%d, wr=%d, ap=%d, rw=%d, bin=%d", r, w, a, u, b) )

	DBG_EXIT
}
#endif
