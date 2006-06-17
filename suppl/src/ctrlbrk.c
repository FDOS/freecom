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

ob(ject): ctrlbrk
su(bsystem): portable
ty(pe): 
sy(nopsis): 
sh(ort description): Install a ^Break handler
he(ader files): 
lo(ng description): Installs a new ^Break handler. Whenever the user
	presses ^Break or ^C DOS invokes an user-installable callback function.
	If a program does not change this callback function, the currently
	active program is usually terminated.\par
	If the passed function pointer is \tok{NULL}, ^Break is ignored totally.\newline
	Otherwise, the ^Break handler invokes the user function. If the user function returns non-zero, the ^Break signal is ignored; otherwise the program is terminated.\newline
	Most compilers support a \tok{longjmp()} from within the ^Break
	callback function.\par
	Note: The handler need not be uninstalled as it is automatically done,
	when the program terminates.
pr(erequistes): 
va(lue): none
re(lated to): _cbreak_catcher_
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): ctrlbrk.c
in(itialized by): 
wa(rning): 
bu(gs): 
co: Micro-C & Pacific HiTech C

*/

#include "initsupl.loc"

#ifdef _MICROC_
#define COMPILE
#else
#ifdef _PAC_NOCLIB_
#define COMPILE
#endif
#endif

#ifdef COMPILE
#include <portable.h>

#ifdef _MICROC_
extern void _cbreak_catcher(void);
#else
extern far void interrupt _cbreak_catcher_ll(void);
extern far void interrupt _cbreak_catcher_ss(void);
#endif

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

#ifdef _PAC_NOCLIB_
extern int (*_fct_cbreak)(void);
int (*_fct_cbreak)(void) = 0;
#else
unsigned _fct_cbreak = 0		; /* addr of fct to invoke */
#endif

void ctrlbrk(inM(int *fct, int (*fct)(void)))
/* Install the ^Break catcher */
{	_fct_cbreak = fct;

#ifndef _MICROC_
#ifdef SMALL_CODE
	set_isrfct(0x23, _cbreak_catcher_ss);
#else
	set_isrfct(0x23, _cbreak_catcher_ll);
#endif
#else
	set_isrfct(0x23, _cbreak_catcher);
#endif
}
#endif
