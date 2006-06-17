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

	FILE *F_dopen(int fd, FLAG *arg)

	Create a FILE* structure for the already opened file descruptor;
	the arg[] array must be initialized with the mode information
	that would be required to open the file via Fopen() normally.

ob(ject): F_dopen
su(bsystem): supplio
ty(pe): L
sy(nopsis): 
sh(ort description): Create a stream for a file descriptor
he(ader files): 
lo(ng description): Creates a new stream, a \tok{(FILE*)} pointer,
	for an already opened file descriptor.\par
	To pass different open mode flags to this function than were
	used when the file descriptor were opened leads to undefined
	behaviour.\newline
	The file descriptor \para{fd} is not tested for validity.
pr(erequistes): \para{arg} != NULL
va(lue): 
re(lated to): 
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
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

FILE *F_dopen(int fd, FLAG *arg)
{	FILE *f;			/* temporary file pointer */

	DBG_ENTER("F_dopen", Suppl_supplio)

	/* Note: The order must match the one of FF_OPENMODE_ARGS! */
#define r arg[0]
#define w arg[1]
#define a arg[2]
#define u arg[3]
#define b arg[4]

	assert(arg);

	DBG_ARGUMENTS( ("fd=%d, flags: rd=%d, wr=%d, ap=%d, rw=%d, bin=%d", fd, r, w, a, u, b) )

	if((f = fopen("nul", u? "rw": r? "r": "w")) != 0) {
		close(Fileno(f));
		Fileno(f) = fd;

		if(b)
			f->FILE_options |= F_BINARY;
		if(a)
			lseek(fd, 0, 0, SEEK_END);
	}

	chkHeap
	DBG_RETURN_P( f)
}
#endif
