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

ob(ject): suppl_testMemChain
su(bsystem): debug
ty(pe): 
sh(ort description): Test the DOS Memory Chain for corruptions
he(ader files): 
lo(ng description): Tests if the DOS Memory Chain had been corrupted and,
	if so, the application is terminated with a message onto screen and
	within logfiles.
pr(erequistes): 
va(lue): none
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dbgm_sr.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <conio.h>
#include <stddef.h>
#endif
#include <portable.h>

#include "mcb.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

#ifndef __GNUC__
#pragma argsused
#endif

#if defined __TURBOC__ && __TURBOC__ <= 0x201
# define offsetof(s_name, m_name) (size_t)&(((s_name*)0)->m_name)
#endif

static int checkItem(void *arg, word segm)
{
    (void)arg;
	if((peekb(segm, offsetof(struct MCB, mcb_type)) != 'M'
	  && peekb(segm, offsetof(struct MCB, mcb_type)) != 'Z')
	 || peekw(segm, offsetof(struct MCB, mcb_size)) >= ~segm) {
		DBG_POP_STATE
		DBG_PRINT(Suppl_class_mem, ("Memory Chain corrupted"))
		cputs("\r\nMemory Chain corrupted!\r\n" 
		 "\7Press any key to terminate application\r\n\7");
		getch();
		DBG_TERMINATE(0)
	}

	return 0;
}

void suppl_testMemChain(void)
{	static volatile  FLAG8 recursive = 1;

	if(--recursive == 0) {	/* prevent recursion */
		DBG_PUSH_STATE
		DBG_CHANGE_STATE("l-")
		mcb_walk(0, checkItem, 0);
		DBG_POP_STATE
	}
	++recursive;
}

