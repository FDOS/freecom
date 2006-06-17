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

ob(ject): invokeDOS
su(bsystem): portable
ty(pe): 
sh(ort description): Call the DOS system API
lo(ng description): Calls the DOS system API interrupt 0x21. The initial
	register values passed to the system are read from the \tok{IREGS}
	structure \para{prp}. On exit,
	the register values are dumped into the same structure overwriting
	the passed-in values.
pr(erequistes): prp != NULL
va(lue): 0: Carry flag was clear on return\item else: AX register, if
	Carry flag was set. Most DOS functions use the Carry flag to
	indicate an error condition and return the error number in the
	register AX, please consult a DOS system API reference, such as
	Ralf Brown's interrupt list, for more details.
re(lated to): IREGS
se(condary subsystems): 
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
#include "suppl.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

#if 0
int invokeDOS(IREGS *prp)
{	DBG_ENTER("invokeDOS", Suppl_portable)
	assert(prp);
	DBG_ARGUMENTS( ("AX=0x%04x", prp->r_ax) )
	int86(0x21, prp, prp);
	DBG_RETURN_BI( (prp->x.cflag)? prp->x.ax: 0)
}
#endif
