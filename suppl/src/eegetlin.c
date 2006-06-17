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

	char *Egetline(FILE *f);

	As Fgetline(), but terminates the program with an error message,
	in case of out-of-memory.

	Return:
		NULL: EOF (or read error);
		else: pointer to line

ob(ject): Egetline
su(bsystem): error
ty(pe): S
sh(ort description): Read the next line from a FILE stream
lo(ng description): Reads the next line from a FILE stream into the heap.
	The length of the line is not directly limited, but it is
	cumulated into a dynamically allocated string. The end of a line
	is indicated by an \tok{'\n'} character or by the end-of-file
	condition. The trailing \tok{'\n'} is \em{not} removed from the string
	and one is appended if it is absent.
	On failure the program is terminates with the error message:
	"Out of memory"
pr(erequistes): 
re(lated to): Eopen Ecreate Fgetline
se(condary subsystems): supplio
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): NULL: end-of-file reached\item else: pointer to dynamically allocated
	buffer with the line, the line always has a trailing \tok{'\n'}
	character
fi(le): eegetlin.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#include <portable.h>
#include "supplio.h"
#include "msgs.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *Egetline(FILE * const f)
{	char *buf;


	DBG_ENTER("Egetline", Suppl_error)

	chkHeap
	if((buf = Fgetline(f)) == 0)
		Esuppl_noMem();
	Eerror(f);

	chkHeap
	if(*buf) 	DBG_RETURN_S( buf)

	/* EOF */
	free(buf);

	chkHeap
	DBG_STRING("EOF reached")
	DBG_RETURN_S( 0)
}
