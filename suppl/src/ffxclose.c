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

	void Fxclose(FILE *fp)

	Close the file and remove the extended attributes from the chain
	added by Fxopen().
	Note: fp can be any FILE* pointer that would be legal to pass
	to fclose(), even those not opened by Fxopen().

ob(ject): Fxclose
su(bsystem): supplio
ty(pe): 
sh(ort description): Close a stream
lo(ng description): Closes the specified stream, regardless, if it
	was opened via Fopen(), \tok{fopen()}, or Fxopen(). If the
	stream has extended attributes attached to it, these are
	handled, too. This is the only way to make sure that a
	temporary file is removed, when it is closed.
pr(erequistes): 
va(lue): none
re(lated to): Fxopen
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#include <assert.h>
#ifndef _MICROC_
#include <stdlib.h>
#endif
#include "supplio.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void Fxclose(FILE *fp)
{	Suppl_ExtAttr *th, *prv;

	DBG_ENTER("Fxclose", Suppl_supplio)

	if(!fp)
		DBG_EXIT

	Fyclose(fp);

	chkHeap
	if((th = F_xinfo(fp, &prv))->sea_fp) {		/* found */
		DBG_INFO( ("extended attributes: fnam=\"%s\", is_temp=%s", th->sea_fnam, th->sea_istmp? "yes": "no") )
		chkHeap
		if(prv)			/* unchain from list */
			prv->sea_nxt = th->sea_nxt;
		else suppl_dfltExtAttr.sea_nxt = th->sea_nxt;

		if(th->sea_istmp) {
			remove(th->sea_fnam);
		}
		chkHeap
		free(th->sea_fnam);
		free(th);
	}

	chkHeap
	DBG_EXIT
}
