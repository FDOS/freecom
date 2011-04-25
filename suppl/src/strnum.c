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

ob(ject): strnum
su(bsystem): cmdline
ty(pe): 
sh(ort description): Parse a numerical option string
lo(ng description): Parses a string of a numeric option. The complete
	supported syntax is:\newline
	/^\s*[+-]?(0x)?[0-9a-f][oqdbh]?\s*$/i
	\item The optional "[+-]" signs indicate that the number as signed.
	In this case
 	the number itself must range between \tok{INT_MIN} and \tok{INT_MAX}.
 	Otherwise the number is \tok{0}..\tok{UINT_MAX}.
 	\item
 	The optional "(0x)" prefix and "[oqdbh]" signs specify the radix of the
 	number:\newlist
 	\item 0x --> hexadecimal (radix == 16), [oqdbh] must not be present
 	\item [oqdbh] --> o & q: octal (radix == 8), d: decimal (radix == 10),
 		b: binary (radix == 2), h: hexadecimal (radix == 16)
 	\endlist
 	The radix defaults to decimal, even if the first digit is \tok{'0'}.
 	\item
 	The numbers itself "[0-9a-f]" can only contain digits valid for the
 	specified radix.
 	\endlist\par
 	If \tok{endp == NULL}, no error position is stored.
pr(erequistes): s != NULL && num != NULL
va(lue): 
 		0: on success, \tok{*endp} unchanged, \tok{*num} contains the number
 \item		else: on failure, the error position is assigned to \tok{*endp}.
 		\tok{*num} contains the value processed so far, zero when no
 			digit was parsed, yet.
re(lated to): strbool atoi strtol
se(condary subsystems): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <ctype.h>
#include <limits.h>
#include "str.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int strnum(const char * const s, int * const num, char ** const endp)
{	char *p, *q;
	int neg, radix, ch;
	unsigned n, maxn, modn;

	DBG_ENTER("strnum", Suppl_cmdline)

	assert(s);
	assert(num);

	DBG_ARGUMENTS( ("str=\"%s\"", s) )

	n = 0;
	p = skipws(s);		/* skip leading whitespaces */

	/* Scan sign */
	maxn = INT_MAX;
	switch(*p++) {
	case '-':	neg = 1;	break;
	default:	--p;
				maxn = UINT_MAX;
	case '+':	neg = 0;	break;
	}

	/* Aquire radix */
	q = skipwd(p) - 1;
	radix = 10;
	if(*p == '0' && toupper(p[1]) == 'X') {	/* C style hexadecimal */
		p += 2;
		radix = 16;
	} else if(p < q) {				/* check the last character */
		switch(toupper(*q)) {
		case 'B':				radix = 2;	break;
		case 'O': case 'Q':		radix = 8;	break;
		case 'D':				radix = 10;	break;
		case 'H':				radix = 16;	break;
		default:	++q;	/* q is the upper limit */
		}
	}
	modn = maxn % radix;	/* maximum digit if n == maxn */
	maxn /= radix;

	--p;
	while(++p < q				/* there is a next character */
	 && n <= maxn) {			/* can add the next character */
			/* transform next ASCII digit into number */
		ch = toupper(*p) - '0';
		if(ch < 0							/* invalid digit < '0' */
		 || (ch > 9 && (ch -= 'A' - ('9' + 1)) < 10)	/* invalid */
		 || ch >= radix						/* invalid greater than radix */
		 || (ch > modn && n == maxn))		/* overflow */
			break;
		n = n * radix + ch;
	}

	if(p == q)					/* ignore trailing whitespaces */
		p = skipws(p);

	*num = neg? -n: n;

	if(!*p)			/* no error */
		DBG_RETURN_I( 0)

	if(endp) *endp = p;
	DBG_RETURN_I( 1)
}
