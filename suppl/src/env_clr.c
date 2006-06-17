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
/*  $RCSfile$
   $Locker$	$Name$	$State$

ob(ject): env_clear
su(bsystem): env
ty(pe): 
sh(ort description): Clear the entire environment
lo(ng description): Clears the environment; on return the environment
	contains neither strings nor variables; the previous contents may be
	garbage
pr(erequistes): 
re(lated to): env_create env_delete env_replace
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): none
fi(le): env_repl.c

 */

#include "initsupl.loc"

#include <suppl.h>
#include "environ.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void env_clear(word segm)
{	
	DBG_ENTER("env_clear", "env")
	DBG_ARGUMENTS( ("env=%u", segm) )

	chkMem

	unless_segm(segm)
		DBG_EXIT		/* no source environment */

	DBG_ARGUMENTS( ("effective env=%u", segm) )

	ENV_CLEAR_SEGM(segm);
	
	chkMem

	DBG_EXIT
}
