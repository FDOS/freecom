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

	Suppl_ExtAttr suppl_dfltExtAttr;

	Default value to be returned bu Fxinfo(); and first element of
	chain of Extended Attribute list.

	Default settings:
		nxt := NULL
		fp := NULL
		fnam := Y_noFNam (usually: "<<unknown filename>>")
		istmp := 0

ob(ject): suppl_dfltExtAttr
su(bsystem): supplio
ty(pe): 
sh(ort description): Default value returned by Fxinfo() on failure
lo(ng description): This variable is the very first item of the chain
	of all extended attributes and will be returned by Fxinfo(), if
	the passed stream has not been opened with Fxopen(), thus,
	it has no extended attributes attached to it. This makes sure that
	functions can use the return value of Fxinfo() without to check for
	the error condition. The values of the extended attributes for
	the default item are:
	\item \tok{fp}: \tok{NULL}
	\item \tok{fnam}: \tok{Y_noFNam}, which usually expands to the
		string "<<unknown filename>>"
	\item \tok{istmp}: \tok{0}, meaning: is not a temporary file
	\endlist
pr(erequistes): 
va(lue): none
re(lated to): Fxopen Fxinfo
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#include "supplio.loc"
#include "msgs.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

Suppl_ExtAttr suppl_dfltExtAttr = {
	iM(int) NULL, NULL, inM(&, (char*)) Y_noFNam
};
