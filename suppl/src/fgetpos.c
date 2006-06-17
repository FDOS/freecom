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

ob(ject): fgetpos
su(bsystem): portable
ty(pe): S
sh(ort description): ISO9899 compatible \tok{fgetpos()} function
lo: ISO9899 compatible \tok{fgetpos()} function to aquire the
	current stream position into a \tok{(fpos_t)} structure.
pr(erequistes): stream != NULL && pos != NULL
va(lue): 0: on succes\item else: on failure
re(lated to): fsetpos FFgetpos
se(condary subsystems): supplio
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): Pacific HiTech C only

*/

#include "initsupl.loc"

#ifdef _PAC_NOCLIB_
#include "supplio.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif


int fgetpos(FILE * const stream, fpos_t * const pos)
{	long p;

	/* The description of ftell() suggests that this function never
		fails?! */
	if((p = ftell(stream)) == -1l)
		return 1;

	*pos = p;
	return 0;
}

#endif
