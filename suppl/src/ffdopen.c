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

	FILE *Fdopen(int fd, char *mode)


ob(ject): Fdopen
su(bsystem): supplio
ty(pe): 
sh(ort description): Create a stream from a file descriptor
lo(ng description): Creates a stream, a \tok{(FILE*)} pointer, for an
	the already opened file descriptor \para{fd};
	the specified mode must match the one of the file descriptor, otherwise
	the behaviour is undefined.
pr(erequistes): \para{mode} != NULL
va(lue): \tok{NULL}: on failure
	\item else: stream / \tok{(FILE*)} pointer
re(lated to): Fopen
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): Micro-C only

*/

#include "initsupl.loc"

#ifdef _MICROC_
#include <stdio.h>
#include <file.h>
#include "supplio.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

FILE *Fdopen(int fd, char *mode)
{	FF_OPENMODE_ARGS

	DBG_ENTER("Fdopen", Suppl_supplio)

	F_openmode(mode, FF_OPENMODE_ARG);

	DBG_RETURN_BP( F_dopen(fd, FF_OPENMODE_ARG))
}
#endif
