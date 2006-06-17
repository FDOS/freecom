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

	char *Egets(const char * const str, size_t size, FILE * const fp);

	As fgets(), but terminate the program on failure

ob(ject): Egets
su(bsystem): error
ty(pe): S
sh(ort description): Read a line from a FILE stream
lo(ng description): Reads a line of maximal \para{size} bytes
	length from a FILE stream and, at end-of-file or error condition,
	the program is terminated
	with the error message: "Cannot read from file \"%s\""
pr(erequistes): 
re(lated to): Eopen Ecreate Fgets Fread
se(condary subsystems): supplio
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): pointer to read characters
fi(le): eegets.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <portable.h>
#include "supplio.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *Egets(char * const str, size_t size, FILE * const fp)
{	char *s;

	DBG_ENTER("Egets", Suppl_error)

	if((s = Fgets(str, size, fp)) == 0)
		Ereaderr(fp);

	chkHeap
	DBG_RETURN_S( s)
}
