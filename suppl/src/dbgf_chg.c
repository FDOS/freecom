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

ob(ject): suppl_log_change
su(bsystem): debug
ty(pe): 
sh(ort description): Change the current state of the logging functions
he(ader files): 
lo(ng description): Changes the current state of the logging functions
	represented by the macros DBG*().
pr(erequistes): More than one debug stack items are present
va(lue): none
re(lated to): suppl_log_lock suppl_log_push
se(condary subsystems): 
in(itialized by): suppl_log_push
wa(rning): 
bu(gs): 
fi(le): dbgf_chg.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <portable.h>

#include "dynstr.h"
#include "str.h"

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void suppl_log_change(suppl_log_sptr_t state)
{	suppl_log_sptr_t p;

	assert(state);

	if(suppl_log_lock()) {
		assert(suppl_Stack);
		p = state;
		while(*p) {
			switch(*p++) {
			case 'd':	/* maximum nesting level */
				strnum(p, &S(maxdepth), (char **)&p);
				break;
			case 'f':	/* files to be logged */
				suppl_log_chg_list(S(ownFiles), S(files), &p);
				break;
			case 'c':	/* functions to be logged */
				suppl_log_chg_list(S(ownFunctions), S(functions), &p);
				break;
			case 'C':	/* classes to be logged */
				suppl_log_chg_list(S(ownClasses), S(classes), &p);
				break;
			case 'l':	/* global log flag */
				suppl_log_flag(S(enabled), &p);
				break;
			case 'i':	/* indention flag */
				suppl_log_flag(S(indent), &p);
				break;
			}
			if(*p++ != ';')
				break;
		}
		suppl_log_unlock();
	}
}
