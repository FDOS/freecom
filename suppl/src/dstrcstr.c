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

	int StrCString(char *str)

	Interprete C-style character strings. Control characters are left
	in the file except the string "\\\n" and a backslash at the end of
	the string, which are cut out of the string.

	Return:
		0: str == NULL
		else: number of bytes str[] now contains; the terminating NUL
			character is counted

ob(ject): StrCString
su(bsystem): dynstr
ty(pe): 
sh(ort description): Expand a C-style string
lo(ng description): Expands a C-style string (backslash escape squences).\par
	The string is overwritten by the expanded string.
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va: number of bytes of \para{str[]} including the \tok{NUL}-byte.
fi(le): dstrcstr.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <ctype.h>
#endif
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int StrCString(char *str)
{	int ch;
	char *p, *h, *q;
	static char fromChar[] = "abnrtv?\"'";
	static char toChar[] = "\7\b\n\r\t\v?\"'";

	DBG_ENTER("StrCString", Suppl_dynstr)
	DBG_ARGUMENTS( ("str=\"%s\"", str) )

	for(q = p = str; (*p++ = ch = *q++) != NUL;)
		if(ch == '\\') {
			if((ch = *q++) == NUL) {
				p[-1] = NUL;
				break;
			}
			if(ch == '\n') 			/* ignore */
				--p;
			else if((h = strchr(fromChar, ch)) != 0)
				p[-1] = toChar[(size_t)(h - fromChar)];
			else if(isodigit(ch)) {
				ch -= '0';
				while(isodigit(*q))
					ch = (ch << 3) | (*q++ - '0');
				p[-1] = ch;
			}
			else if((ch == 'x' || ch == 'X') && isxdigit(*q)) {
				ch = 0;
				do ch = (ch << 4) | toxdigit(*q);
				while(isxdigit(*++q));
				p[-1] = ch;
			}
			else p[-1] = ch;
		}
	DBG_RETURN_I( (size_t)(p - str))
}
