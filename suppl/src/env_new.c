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

ob(ject): env_create
su(bsystem): env
ty(pe): 
sh(ort description): Create a new environment
lo(ng description): Creates a new environment of a given length
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): \tok{0}: failure; out of memory (ENOMEM), parameter out of range (EINVAL)
	\item else: address of new environment
fi(le): env_new.c

*/

#include "initsupl.loc"

#include <portable.h>
#include "environ.h"
#include "suppl.h"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

word env_create(const unsigned length)
{	word segm;
	int allocMode;

	DBG_ENTER("env_create", Suppl_env)
	DBG_ARGUMENTS( ("len=%u", length) )

	chkMem
	
	if(length == 0) {
		eno_set(EINVAL);
		return 0;
	}

	switch(env_resizeCtrl & (ENV_FIRSTFIT | ENV_LASTFIT)) {
		case ENV_FIRSTFIT:	allocMode = 0; break;/* first fit */
		case ENV_LASTFIT:	allocMode = 2; break;/* last fit */
		default:			allocMode = 1; break;/* best fit */
	}

	DBG_ARGUMENTS( ("allocMode=%u, use UMB?=%u", allocMode, !!(env_resizeCtrl & ENV_USEUMB)) )

	/* try to allocate block in the UMBs */
	if(!(env_resizeCtrl & ENV_USEUMB)
	 || (segm = allocBlk(length, allocMode | 0x40)) == 0)
		segm = allocBlk(length, allocMode);	/* try the low memory */

	chkMem
	
	if(segm)	poked(segm, 0, 0);	/* initialize to an unused environment */
	else		eno_set(ENOMEM);

	chkMem
	
	DBG_RETURN_U( segm)
}
