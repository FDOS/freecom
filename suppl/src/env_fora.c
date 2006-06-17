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

ob(ject): env_forall
su(bsystem): env
ty(pe): 
sh(ort description): Enumerate all variables
lo(ng description): Enumerates all variables of a given environment and
	invokes a function for each found variable. The callback function must
	be of type \tok{ENV_WALKFUNC}, which prototype is:
	\example{|}
	|int FCT(void *arg, word env, word offset)
	The argument \para{arg} is passed unchanged to the callback function and
	may be used to pass arbitary information from the program via \tok{env_forAll} to the callback function.\newline
	If the callback function returns \tok{0} (zero), the enumeration process
	continues; otherwise the loop breaks immediately.\par
	On loop, \tok{env_forAll} continues with the next variable following
	the one starting at the position passed to the callback function.
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): \tok{-1}: parameter out of range
		\list \tok{ENXIO}: No environment at all
			\item \tok{EINVAL}: invalid argument
		\endlist
	\item \tok{0}: all variables enumerated, callback function returned \tok{0}
	(zero) every time
	\item else: the return value of the callback function
fi(le): env_fora.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#endif
#include <portable.h>
#include "environ.loc"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int env_forAll(word segm, ENV_WALKFUNC fct, void *arg)
{	word ofs;
	int retVal;

	DBG_ENTER("env_forAll", Suppl_env)
	DBG_ARGUMENTS( ("env=%u", segm) )

	chkMem
 	unless_segm(segm) {
 		eno_set(ENXIO);
 		return -1;
 	}
 	if(!fct) {
 		eno_set(EINVAL);
 		return -1;
 	}
  	
	DBG_ARGUMENTS( ("effective env=%u", segm) )

	for(ofs = 0; peekb(segm, ofs); ofs += env_varlen(segm, ofs)) {
		DBG_INFO( ("invoke callback function with (%u:%u)", segm, ofs) )
		if((retVal = (*fct)(arg, segm, ofs)) != 0	/* call function & break */
		 || !peekb(segm, ofs))		/* callback function cleared env */
			DBG_RETURN_I( retVal)
	}

	DBG_RETURN_I( 0)
}
