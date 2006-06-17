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

	void Fsetmode(FILE *fp, int mode);

	Switches between Read and Write mode, as the Micro-C implementation
	of R&W file access is heavily buggy

	Micro-C only

ob(ject): Fsetmode
su(bsystem): supplio
ty(pe): 
sh(ort description): Switch a stream into read or write mode
lo(ng description): Switches a stream into \enum read: \tok{mode == FF_RD},
	\enum write: \tok{mode == FF_WR}, or
	\enum read&write: \tok{mode == FF_RW}
	\endlist
	mode.\par
	The read&write mode of Micro-C is heavily buggy, therefore care must
	be taken if the stream is positioned before the physical end-of-file.
	The following rules are recommended to handle R&W access savely:
	\enum Open the stream in \tok{"rw"} (old style), or \tok{"r+"}, \tok{"w+"}
	or \tok{"a+"} (ANSI style).
	\enum Immediately after to open the stream
	perform \tok{Fsetmode(FF_RD)} to bring the stream in read-only mode.
	The stream can savely be seeked now.
	\enum Enclose all functions, that actually shall write something into
	the stream, into:
	\example{|}
	|Fsetmode(stream, FF_RW);
	|// do write into stream, but NEVER seek or read or flush the stream
	|Fsetmode(stream, FF_RD);
	\endlist
	\par\em{Note:} To sequentially write the file no precausations need to
	be taken, but one cannot read from, seek into or flush a stream, that
	has the write mode enabled, savely.
	\par\em{Also note:} This function does not modify the access permissions
	granted by the system, it sticks to the logical flags kept within the
	\tok{(FILE)} structure and maintained by Micro-C's standard C library.
pr(erequistes): fp != NULL
va(lue): none
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): Micro-C only

*/

#include "initsupl.loc"

#ifdef _MICROC_
#include <assert.h>
#include <stdio.h>
#include <portable.h>
#include "supplio.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void Fsetmode(FILE *fp, int mode)
{
	assert(fp);

	fflush(fp);
	switch(mode) {
	case FF_RD:	fp->FILE_options &= ~F_WRITE; break;
	default: fp->FILE_options |= F_WRITE; break;
	}
}
#endif
