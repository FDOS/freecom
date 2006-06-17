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

ob(ject): suppl_log_chg_list_
su(bsystem): debug
ty(pe): H_
sy(nopsis): 
sh(ort description): Handle a change state item modifying a list
he(ader files): 
lo(ng description): Handles a change state item that modifies a list
	of names.\par
	When a new state is created by using a push_state function,
	the old lists are not duplicated, but their references only. To
	indicate that a state has modified a list and therefore has an own
	copy of it, a flag is maintained.\par
	The change state item consists of a comma-seperated list of names:
	\tok{?****}, with:
	\item \tok{****} is the name, and
	\item \tok{?} specifies if to add \tok{'+'} or remove \tok{'-'} the name.
	\endlist
	If no name is specified, all current entries are removed and the
	default behaviour is changed to except everything \tok{'+'} or
	deny all \tok{'-'}.
pr(erequistes): str != 0 && *str != 0 && flag != 0 && list != 0
va(lue):\endlist none, but
	\item \tok{*str :=} pointer to next semicolon or end of string
	\item \tok{*flag := 1}
	\item \tok{*list}: modified according specified items
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
#include <string.h>
#endif
#include <portable.h>

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void suppl_log_chg_list_(FLAG *flag
	, suppl_log_list_t *list
	, suppl_log_sptr_t *str)
{	suppl_log_sptr_t p, q;
	suppl_log_list_t *act, *prv;
	int flg;

	assert(flag);
	assert(list);
	assert(str);
	assert(*str);

	if(*(p = *str) == NUL || *p == ';')		/* Nothing to do */
		return;

	if(!*flag) {				/* Must create a local copy */
		suppl_log_dup_list(list);
		*flag = 1;
	}

	/* Now append the new entries */
	chkHeap
	do {
		switch(flg = *p) {
		case '+': case '-':
			++p;
			break;
		default:
			flg = '+';				/* default to "enabled" */
			break;
		}
		if((q = strpbrk(p, ";,")) == 0)	/* advance to end of name */
			q = strend(p);
		if(q == p) {				/* default specified */
			suppl_log_del_list_(list);
			list->suppl_l_name[0] = flg;
		}
		else {
			act = list;
			while((act = (prv = act)->suppl_l_nxt) != 0) {
				if(suppl_log_match_item(act, p)) {
					/* remove the current item from the list */
					chkHeap
					prv->suppl_l_nxt = act->suppl_l_nxt;
					free(act);
					chkHeap
					act = prv;
				}
			}

			/* Append a new item */
			assert(prv);
			prv->suppl_l_nxt = suppl_log_mkf_item(flg, p, (size_t)(q - p));
		}

		chkHeap
		p = q;
	} while(*p++ == ',');

	*str = p - 1;
	chkHeap
}
