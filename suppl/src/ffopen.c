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

	FILE *Fyopen(char *fnam, char *mode)

	Open a new file; as standard

	Micro-C only

ob(ject): Fyopen
su(bsystem): supplio
ty(pe): 
sh(ort description): \tok{fopen()} according ANSI C
lo: Re-implementation of \tok{fopen()}, because Micro-C's one
	does not comply to the ANSI C spec (ISO9899).\par
	The macro \tok{Fopen} maps to \tok{Fyopen()}, if
	\tok{SUPPL_USE_EXTENDED_ATTRIBUTES} is not defined.
pr(erequistes): fnam != NULL && mode != NULL
va(lue): \tok{NULL}: on failure\item else: stream / \tok{(FILE*)} pointer
re(lated to): Fopen Fxopen Fclose
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
/* #include <portable.h> */
#include "supplio.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

FILE *Fyopen(char *fnam, char *mode)
{	FF_OPENMODE_ARGS	/* defines r,w,a,u,b --> read/write/append/
							update/binary */
	FILE *f;			/* temporary file pointer */
	int fd;				/* temporary file descriptor */

	DBG_ENTER("Fyopen", Suppl_supplio)

	assert(fnam);
	assert(mode);

	DBG_ARGUMENTS( ("fnam=\"%s\", mode=\"%s\"", fnam, mode) )

	F_openmode(mode, FF_OPENMODE_ARG);

	if(r | a) {		/* Try to open the existing file */
		asm "mov dx, 6[bp]";
		w? (int)r + 0x3d21: 0x3d40;	/* share: deny write if write access */
		asm {
			int 21h
			jnc ???okOpen
		}
	}

	chkHeap
	if(w) {		/* Try to create new file */
		asm {
			mov dx, 6[bp]
			mov cx, 0
			mov ah, 3ch
			int 21h
			jc ???errCreat
		}
		fd = nargs();
		if(!u)			/* If w & r --> u is true, see above */
			goto ok;
		/* currently opened for write only
			--> re-open it properly */
		close(fd);
		asm {
			mov dx, 6[bp]
			mov ax, 3d22h		;; R&W access, share deny write
			int 21h
			jnc ???okOpen
		}
		remove(fnam);
	}
asm "???errCreat:";
	chkHeap
	DBG_RETURN_P( 0)		/* open failed, as we could not open the file */

asm "???okOpen:";
		fd = nargs();
ok:
	if((f = F_dopen(fd, FF_OPENMODE_ARG)) == 0) {
		/* Cannot create FILE structure */
		close(fd);
		DBG_RETURN_P( 0)
	}

	chkHeap
	DBG_RETURN_P( f)
}
#endif
