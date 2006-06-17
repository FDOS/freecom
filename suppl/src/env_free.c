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

ob(ject): env_endOfVars
su(bsystem): env
ty(pe): L
sh(ort description): Return the first byte behind the environment variables
lo(ng description): Returns the offset of the first byte of the environment
	that is not a part of the variables area.
pr(erequistes): \para{segm} != 0
re(lated to): 
se(condary subsystems): 
in(itialized by): 
bu(gs): 
va(lue): offset of first byte behind the environment variables
fi(le): env_free.c

ob(ject): env_firstFree
su(bsystem): env
ty(pe): L
sh(ort description): Return the first byte not used by the environment areas
lo(ng description): Returns the offset of the first byte of the environment
	that is unused currently.
pr(erequistes): \para{segm} != 0
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): This offset may point outside of the environment, if it is
	completely filled.
bu(gs): 
va(lue): offset of first unused byte of the environment
fi(le): env_free.c

ob(ject): env_freeCount
su(bsystem): env
ty(pe): L
sh(ort description): Count the number of unused bytes of the environment
lo(ng description): Returns the number of unused bytes of the environment.
pr(erequistes): \para{segm} != 0
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): If the segment is >= 64KB long and is filled >= (64KB - 1), this
	function returns invalid values.
va(lue): offset of first unused byte of the environment
fi(le): env_free.c

 */

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <string.h>
#endif
#include <portable.h>
#include "environ.h"
#include "mcb.h"
#include "fmemory.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

word env_endOfVars(const word segm)
{	word offset;

	DBG_ENTER("env_endOfVars", Suppl_env)
	DBG_ARGUMENTS( ("env=%u", segm) )

	chkMem
	
	assert(segm != 0);

	offset = 0;					/* env starts always at location 0 */

	while(peekb(segm, offset))	/* there is a variable */
		offset += env_varlen(segm, offset);

	DBG_RETURN_U( offset)				/* This is the terminator byte */
}

word env_firstFree(const word segm)
{	DBG_ENTER("env_firstFree", Suppl_env)
	DBG_ARGUMENTS( ("env=%u", segm) )

	chkMem
	
	assert(segm != 0);

	DBG_RETURN_BU( env_string(segm, env_strings(segm)))
}

word env_freeCount(const word segm)
{	DBG_ENTER("env_firstCount", Suppl_env)
	DBG_ARGUMENTS( ("env=%u", segm) )

	chkMem
	
	assert(segm != 0);

	DBG_RETURN_BU( mcb_length(segm) - env_firstFree(segm))
}
