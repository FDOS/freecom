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

ob(ject): dfnsqueeze
su(bsystem): dfn
ty(pe): H
sh(ort description): Squeeze the given filename
he(ader files): 
lo(ng description): Flips forward slashes into backslashes, squeezes multiple
	backslashes into a single one (unless the leading two for UNC paths),
	all characters are upper-cased (in order to support LFN in the future
	one should not rely on upcased characters)
	and removes any "." components except the very last.\newline
	Note: On the future ".." might be removed, too!
pr(erequistes): 
va(lue): NULL: on failure (ENOMEM)\item else: pointer to dynamically allocated
	squeezed filename
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dfnsquee.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#endif
#include "eno.loc"
#include "dfn.loc"

#include "dynstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *dfnsqueeze(const char * const fnam)
{	char *p, *h, *q;

#ifdef SUPPORT_UNC_PATH
	DBG_ENTER("dfnusqueeze", Suppl_dfn)
#else
	DBG_ENTER("dfnsqueeze", Suppl_dfn)
#endif
	DBG_ARGUMENTS( ("fnam=\"%s\"", fnam) )

	if(!fnam || (h = q = eno_strdup(fnam)) == 0)
		DBG_RETURN_S( 0)

#ifdef SUPPORT_UNC_PATH
	if(isUNCpath(q))		/* keep the two backslashes */
		q += 2;
#endif

	p = q;						/* where to begin to squeeze */

/* First: Flip the slashes */
	while((q = strchr(q, '/')) != 0)
		*q = '\\';

/* Second: Squeeze & upcase */
	
	q = p;

	chkHeap
	do {
redo:
		if(*q == '\\') {	/* possibly to be squeezed */
			if(q[1] == '\\') {		/* Squeeze '\\\\' -> '\\' */
				++q;
				goto redo;
			}
			if(q[1] == '.') {
				if(q[2] == '\\') {		/* squeeze '\\.\\' -> '\\' */
					q += 2;
					goto redo;
				}
#if 0			/* this may lead to confusion -- 2000/06/07 ska*/
				if(q[2] == '\0') {		/* squeeze '\\.\0' -> '\\' */
					*p = '\\';
					*++p = NUL;
					break;
				}
#endif
			}
		}
	} while((*p++ = toFUpper(*q++)) != 0);

	chkHeap
	DBG_RETURN_BS( StrTrim(h))
}
