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

ob(ject): env_delete
su(bsystem): env
ty(pe): 
sh(ort description): Delete an environment segment
lo(ng description): Deletes the given environment segment.\par
	If it is \tok{0} (zero) or the default environment, the field of
	the PSP is updated with \tok{0} (zero) to indicate that no default
	environment exists.
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): none
fi(le): env_del.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#endif
#include <portable.h>
#include "environ.loc"
#include "suppl.h"
#include "mcb.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void env_delete(word segm)
{	DBG_ENTER("env_delete", Suppl_env)
	DBG_ARGUMENTS( ("env=%u", segm) )

	chkMem
 	unless_segm(segm)
 		DBG_EXIT
  
 	DBG_ARGUMENTS( ("effective env=%u", segm) )
  
 	pokew(SEG2MCB(segm), MCB_OFF_OWNER, _psp);
 	freeBlk(segm);
 	chkMem
 	/* Make sure the deleted segment won't be referenced */
 	env_relocateSegment(segm, 0);
  

	DBG_EXIT
}
