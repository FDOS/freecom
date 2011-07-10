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

ob(ject): fputmc
su(bsystem): supplio
ty(pe): 
sh(ort description): Dump the same character multiple times
lo(ng description): Writes the same character multiple times into
	the stream.\par
	If \tok{cnt == 0}, nothing happens.
pr(erequistes): f != NULL
va(lue): \para{ch}: on success\item \tok{EOF}: on failure
re(lated to): putc
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#include <stdio.h>
#include <string.h>
#include <portable.h>

#include "suppldbg.h"

#ifdef SUPPL_LOG_FUNCTION
#include <ctype.h>
#endif

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int fputmc(int ch, int cnt, FILE *f)
{	int i;
	char buf[80];
	DBG_ENTER("fputmc", Suppl_supplio)

	assert(f);

	DBG_ARGUMENTS( ("ch='%c' (0x%02x), cnt=%u", isprint(ch)? ch: ' ', ch & 0xff, cnt) )

	if(!cnt) DBG_RETURN_I( ch)

	i = sizeof(buf)-1;
	if (cnt < sizeof(buf)-1) i = cnt;
	memset(buf, ch, i);
	buf[i] = '\0';
	while(cnt > 0) {
		if(fprintf(f, "%s", buf) == EOF)
			DBG_RETURN_I( EOF)
		cnt -= sizeof(buf)-1;
		if (cnt < sizeof(buf)-1) buf[cnt] = '\0';
	}

	DBG_RETURN_I( ch)
}
