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

ob(ject): Fgetline
su(bsystem): supplio
ty(pe): 
sh(ort description): Read the next line from a steam
lo(ng description): 
	Read the next line from stream \para{f} into a dynamically adjusted buffer
	 The maximal length of one line must not exceed \tok{INT_MAX}.\par
	There is \em{always} a \tok{'\n'} (newline) character at the end of the
	line. That means empty lines show up as \tok{"\n"}.\par
	The user must free the buffer.\par
	No action is performed to actually detect end of line conditions,
	that means if the line is \em{not} finished with the \tok{'\n'} character,
	the behaviour is compiler-specific; but if this function recieves a
	line without an appended newline character, one is appended. Latter
	condition should be encountered only at the end of stream.
pr(erequistes): f != NULL
va(lue): 
		\tok{NULL}: memory allocation error
		\item else: pointer to dynamically allocated buffer holding the line.
			If the line is "", EOF (or a read error) was encountered.
re(lated to): Fread Fgets
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
		In Micro-C \tok{fgets()} does not return the \tok{'\n'} at the end of
		the line. At least in version v3.15 after reading \tok{sizeof(buf)-1}
		characters \tok{fgets()} stops and returns; thus, if the inputline
		has \em{exactly} \tok{sizeof(buf)-1} character in it, \tok{fgets()} returns
		with the buffer completely filled and leaves the \tok{'\n'} in the
		stream. The next time \tok{fgets()} is called, an empty line is
		returned. This behavious makes it possible to determine the
		end of the line on behalf of the \em{fullness} of the line,
		meaning if the buffer is filled completely, one can savely
		assume that the line continues.
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#include <string.h>
#endif
#include <portable.h>
#include "supplio.loc"
#include "eno.loc"

#ifndef _MICROC_
#define BUFCHANGE 128
#else
#define BUFCHANGE 256
#endif

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *Fgetline(FILE * const f)
{	char *buf, *p;
	size_t len;

	DBG_ENTER("Fgetline", Suppl_supplio)

	assert(f);

	/* The buffer is allocated in chunks of BUFCHANGE bytes.
		The loop tries to read BUFCHANGE bytes into the last
		allocated chunk. If the fgets() call fails or returns
		with '\n' attached to the line, Fgetline() assumes that
		the line was read completely.
	*/

	chkHeap
	if((p = buf = eno_malloc((len = BUFCHANGE) + 1)) == 0) {
		DBG_INFO( ("Out of memory allocating %u bytes", len) )
		DBG_RETURN_S( 0)
	}

	*p = '\0';

	chkHeap
	while(	fgets(p, BUFCHANGE, f)
	 && (p = strchr(p, '\0')) == buf + len - 1
	 && p[-1] != '\n') {
	 	chkHeap
	 	if((int)(len += BUFCHANGE) <= 0		/* (int) range overflow */
	 	 || (p = eno_realloc(buf, len + 1)) == 0) {	/* heap error */
	 	 	if((int)len < 0)
	 	 		eno_set(ERANGE);
	 	 	free(buf);
			DBG_INFO( ("Out of memory allocating %u bytes", len) )
			DBG_RETURN_S( 0)
	 	}
	 	buf = p;
	 	p += len - BUFCHANGE - 1;
	}

	chkHeap
	if(p != buf)	/* at least one character at line -->
									no EOF --> must terminate in '\n' */
		if(p[-1] != '\n') {	/* add the newline character */
			/* as one byte was allocated more than necessary, the
				slot is already available */
			*p = '\n';
			*++p = '\0';
			DBG_STRING("Missing newline character appended")
		}

		/* Shrink the block to its smallest size */
	chkHeap
	DBG_RETURN_BS( (p = realloc(buf, (size_t)(p - buf) + 1)) == 0
		? buf		/* failed for shrinking ?! */
		: p)
}
