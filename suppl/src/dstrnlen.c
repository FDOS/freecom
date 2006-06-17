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

	size_t StrnLen(const char * const s, size_t len)

	Return the length of the string, but ensures to break after
	len bytes.

	if s == NULL, return 0

ob(ject): StrnLen
su(bsystem): dynstr
ty(pe): 
sh(ort description): Return the length of a string
lo(ng description): Returns the length of a string, but ensures that the
	function returns after at most \para{len} bytes. This is useful to
	prevent that a normal \tok{strlen()} function endlessly wraps the
	segment boundary without hitting a \tok{NUL}-byte once.
pr(erequistes): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va: Number of character of the string, but at most \para{len}
fi(le): dstrnlen.c

*/

#include "initsupl.loc"

#include <portable.h>

#include "dynstr.h"


#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif /*#	defined(RCS_Version) */

/*
 *	Duplicate a portion of a string
 */
size_t StrnLen(const char * const s, size_t len)
{	const char *p;
	size_t l;

	DBG_ENTER("StrnLen", Suppl_dynstr)

	if(!s) DBG_RETURN_U( 0)

	p = s - 1;
	l = (size_t)-1;
	while(++l < len && *++p);

	DBG_RETURN_U( l)
}
