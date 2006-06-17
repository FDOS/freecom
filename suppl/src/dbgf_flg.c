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

ob(ject): suppl_log_flag_
su(bsystem): debug
ty(pe): H_
sy(nopsis): 
sh(ort description): Handle a change state item modifying a flag
he(ader files): 
lo(ng description): Handles a change state item that modifies a flag.\par
	The flag can be: \tok{'+'} or \tok{"on"} or \tok{"1"} (Note: the
	\em{string} "1", but the numerical "one") meaning \em{enabled} and
	\tok{'-'} or \tok{"off"}  or \tok{"0"} meaning \em{disabled}.\newline
	No localization support is implemented for these values as they seem
	to be related to the programmer only. That means that one cannot use
	the localized variant of \tok{"on"} as defined for the \subsys{cmdline}.
pr(erequistes): str != NULL && *str != NULL && flag != NULL
va(lue):\endlist none, but
	\item \tok{*str :=} pointer to next semicolon or end of string
	\item \tok{*flag :=} specified boolean state
re(lated to):  suppl_log_push suppl_log_pop suppl_log_change_state
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <portable.h>

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void suppl_log_flag_(FLAG *flag, suppl_log_sptr_t *str)
{	suppl_log_sptr_t p;
	int ch, flg;

	assert(flag);
	assert(str);
	assert(*str);

	if((ch = *(p = *str)) == NUL || ch == ';')		/* Nothing to do */
		return;

	flg = 0;
	switch(*(p = *str)) {
	case '\0': case ';':			/* Nothing to do */
		break;
	case 'o': case 'O':				/* on or off */
		switch(*++p) {
		case 'n': case 'N':
			goto flgEnable;
		case 'f': case 'F':
			if((ch = *++p) == 'f' || ch == 'F')
				goto flgFound;
		}
		break;						/* failed */
	case '+': case '1':
	flgEnable:	flg = 1;
	case '-': case '0':
	flgFound:	/* Now the change item must be finished */
		if((ch = *++p) == NUL || ch == ';') {
			*str = p;
			*flag = flg;
		}
	}

}
