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

ob(ject): env_resize
su(bsystem): env
ty(pe): 
sy(nopsis): 
sh(ort description): Resize an environment
he(ader files): 
lo(ng description): Resizes an environment by the amount of bytes
	specified via \para{delta}. The contents is retained, except if the
	environment shrinks below the last used byte.\par
	If \tok{\para{delta} == 0}, the length of the environment
	is returned rather than its address. The environment itself remains
	unchanged.\par
 	If the block shrinks below the currently first free byte limit or
 	the block must be moved, the global variable \tok{env_resizeCtrl} controls
 	various special modes.\par
 	If the block is resized below the first free byte limit, the
 	contents of the environment is most probably destroyed and should
 	be totally cleared before it is used.
pr(erequistes): 
va(lue): 0: on failure; out-of-memory (ENOMEM), if EZERO, \tok{\para{delta} == 0} and
	the length of the source environment is zero
	\item else: if \tok{\para{delta} == 0}, the
	size of the environment segment; otherwise the possibly new
	segment address of the environment
re(lated to): env_resizeCtrl env_setsize
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers):

ob(ject): env_setsize
su(bsystem): env
ty(pe): 
sy(nopsis): 
sh(ort description): Set the size of an environment
he(ader files): 
lo(ng description): Resizes an environment to the amount of bytes
	specified via \para{newLength}. The contents is retained, except if the
	environment shrinks below the last used byte.\par
	If \tok{\para{newLength} == 0}, the environment segment is deallocated.\par
	See \tok{env_resize} for more information.
pr(erequistes): 
va(lue): 0: on out-of-memory (ENOMEM) or environment was deleted (EZERO)\item else: 
	 the possibly new segment address of the environment
re(lated to): env_resizeCtrl env_resize
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
#include <dos.h>
#endif
#include <portable.h>
#include "environ.loc"
#include "mcb.h"
#include "suppl.h"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

word env_setsize(word Osegm, unsigned newLength)
{	unsigned segm, length;

	DBG_ENTER("env_setsize", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, new length=%u", Osegm, newLength) )

	chkMem

	if(newLength == 0) {		/* remove segment */
		env_delete(Osegm);
		eno_set(EZERO);
		DBG_RETURN_I( 0)
	}

 	env_Xsegm(segm, Osegm);
 	length = mcb_length(segm);		/* returns 0 on segm == 0 */


	DBG_ARGUMENTS( ("effective env=%u, cur len=%u", segm, length) )

	if(length > newLength) {		/* shrink the block */
		/* Because length > newLength > 0 --> segm != 0 */
		DBG_STRING("Shrink block")
		length = env_firstFree(segm);
		resizeBlk(segm, (length > newLength
				 && !(env_resizeCtrl & ENV_SHRINKBELOW))
		  ? length		/* don't shrink below last used byte */
		  : newLength);
		/* shrinking cannot fail --> ignore return value */
	}
 	else if(length != newLength) {
 		if(segm) {
 			 DBG_STRING("Grow block")
 			 if(resizeBlk(segm, newLength)) {
 				 /* Grow failed --> try to allocate a new block */
 			 	if(!(env_resizeCtrl & ENV_ALLOWMOVE)) {
 			 			/* fail as to allocate is not allowed */
 			 		eno_set(ENOMEM);
 			 		DBG_RETURN_U(0)	
				}
 				segm = env_replace(Osegm
 					  , ENV_DELETE | ENV_COPY | ENV_CREATE, newLength);
 			 		/* On success, the replace() function already updates
 			 			glbSeg or dfltSeg if applicable */
 			 }
 		} else {
 			DBG_STRING("Create new environment")
 			if((segm = env_create(newLength)) != 0) {
 				/* This point is reached only, if setsize() has been called
 					with Osegm == dlftSeg == glbSeg == 0.
 					Therefore it is assumed that the global segment is to
 					be set */
 				env_setGlbSeg(segm);
 				DBG_INFO(("New global environment: %04x", segm))
  			}
  		}
  	}
	chkMem
	DBG_RETURN_U( segm)		/* return the segment address */
}

word env_resize(word segm, int delta)
{	unsigned length;

	DBG_ENTER("env_resize", Suppl_env)
	DBG_ARGUMENTS( ("env=%u, delta=%d", segm, delta) )

	length = mcb_length(env_segment(segm));

	if(delta == 0) {	/* it's a request of the current length */
		eno_set(EZERO);
		DBG_RETURN_U( length)
	}

	/* honor the arethmetic (int) warp in the expressions */
	if(delta < 0)		/* shrink block */
		DBG_RETURN_BU( env_setsize(segm, length <= -delta? 0: length + delta))

	/* grow block */
	DBG_RETURN_BU( env_setsize(segm
	 , inM(65535, 65535U) - length < (unsigned)delta? inM(65535, 65535U)
	 	: length + (unsigned)delta))
}
