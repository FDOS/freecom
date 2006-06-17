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

ob(ject): strtol
su(bsystem): portable
ty(pe): 
sh(ort description): Transform a number from ASCIZ --> binary format
lo(ng description): Transforms a number from ASCIZ into binary
	format.\par
	If \tok{radix == 0}, the number is interpreted in C-style.\newline
	If \tok{endp == NULL}, the erroreous end position is not saved.
pr(erequistes): number != NULL && 0 <= radix <= 36 && radix != 1
va(lue): 0: on error (\tok{*endp} updated), or number == 0 (\tok{*endp}
	\em{not} updated)
	\item else: transformed number 
re(lated to): atoi
se(condary subsystems): 
bu(gs): Because Micro-C does not support \tok{(long)} numbers, the
	result is \tok{(int)}.
co(mpilers): Micro-C only

*/

#include "initsupl.loc"

#ifdef _MICROC_
#include <ctype.h>
#include <stdio.h>

int strtol(char *nr, char **endp, int radix)
{	int i, negate;
	register int c;

	i = 0;
	if((negate = (c = *(nr = skipws(nr))) == '-') || c == '+')
		nr = skipws(nr + 1);

	if(radix == 0) 
		if(*nr == '0') {
			if(tolower(nr[1]) == 'x') {
				radix = 16;
				nr += 2;	/* skip 0x */
			}
			else radix = 8;	/* don't skip '0', mayhap its only "0" */
		}
		else radix = 10;

	if(radix < 2 || radix > 36 || !*nr) {
		if(endp)	*endp = nr;
		return 0;		/* indicate error condition */
	}

	--nr;
	while((c = *++nr) != 0 && isalnum(c)) {
		if(isdigit(c))
			 c -= '0';
		else c = toupper(c) - 'A' + 10;

		if(c >= radix)		/* invalid number */
			break;

		i = i * radix + c;
	}

	if(*(nr = skipws(nr)) != '\0') {		/* failed */
		if(endp)	*endp = nr;
		return 0;
	}

	return negate? -i: i;
}
#endif
