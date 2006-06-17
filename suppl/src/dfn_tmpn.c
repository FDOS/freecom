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

ob(ject): dfn_tmpnam
su(bsystem): dfn
ty(pe): 
sh(ort description): Create an eight-byte temporary filename
he(ader files): 
lo(ng description): Creates an eight-byte temporary filename based on the
	current date and time, no \tok{NUL} terminator byte is appened nor is
	one of the eight bytes a \tok{0} (zero).\par
	Currently the eight characters are constructed as follows to hopefully
	construct a none-existing temporary filename the first try:\newline
	filename: "01234567"
	\item	0: day (DOS-2A, DL)
		\item 1: month (DOS-2A, DH)
		\item 2: year % 256 (DOS-2A, CL)
		\item 3: counter % 36 [each time this function is called, this counter
			is incremented]
		\item 4: counter / 36
		\item 5: second (DOS-2C, DH)
		\item 6: minute (DOS-2C, CL)
		\item 7: hour (DOS-2C, CH)
	\endlist
	The bytes are encoded as following:\newline
		byte % 36	:	character\newline
		0..25		:	A-Z\newline
		26..35		:	0-9\newline
pr(erequistes): \para{fnam[]} must be at least 8 bytes long
va(lue): The first eight bytes of \para{fnam[]} are overwritten with the
	filename, no \tok{'\0'} byte is appened
re(lated to): dfntmpnam dfntmpfile
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dfn_tmpn.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <portable.h>

#include "dfn.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

static digit(int v)
{	v %= 36;
	return v < 26? 'A' + v: v + '0' - 26;
}

void dfn_tmpnam(char * const fnam)
{	struct REGPACK r;
	static int cnt = 0;
	int c;

	DBG_ENTER("dfn_tmpnam", Suppl_dfn)
	DBG_ARGUMENTS( ("cnt=%u", cnt) )

	assert(fnam);

	/* counter */
	fnam[3] = digit(c = ++cnt);	/* local copy for reentrant calling */
	fnam[4] = digit(c / 36);

	/* date */
	r.r_ax = 0x2a00;
    intr( 0x21, &r );
	fnam[2] = digit(r.r_cx);
	fnam[1] = digit(r.r_dx >> 8);
	fnam[0] = digit(r.r_dx);

	/* time */
	r.r_ax = 0x2c00;
	intr(0x21, &r);
	fnam[7] = digit(r.r_cx >> 8);
	fnam[6] = digit(r.r_cx);
	fnam[5] = digit(r.r_dx >> 8);

	DBG_RETURN_VALUES(("fnam[]=\"%c%c%c%c%c%c%c%c\""
		, fnam[0]
		, fnam[1]
		, fnam[2]
		, fnam[3]
		, fnam[4]
		, fnam[5]
		, fnam[6]
		, fnam[7]) )

	DBG_EXIT
}
