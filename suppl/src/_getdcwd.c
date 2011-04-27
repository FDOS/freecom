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

ob(ject): _getdcwd
su(bsystem): portable
ty(pe): H
sy(nopsis): 
sh(ort description): Return the fully-qualified current working path of a drive
he(ader files): 
lo(ng description): Retrieves the fully-qualified current working
	directory of a specified drive. The path does not contain a trailing
	backslash unless it is the root directory. The path does contain the
	drive letter.\par
	If \para{buf} == \tok{NULL}, a new buffer of size \para{length} is
	allocated.
pr(erequistes): 
va(lue): NULL: on error
	\list \tok{ENOMEM}: out of memory
	\item \tok{ERANGE}: Buffer of getdcwd() too small
	\item warning: There is no change of errno, if the DOS API call fails!!
	\endlist
	\item else: pointer to path
re(lated to): getdisk dfnexpand
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): _getdcwd.c
in(itialized by): 
wa(rning): 
bu(gs): 
co: Micro-C, Pacific HiTech C

*/

#include "initsupl.loc"
#include "../../config.h"
#undef FEATURE_LONG_FILENAMES

#ifdef _MICROC_
#define COMPILE 1
#endif
#ifdef _PAC_NOCLIB_
#define COMPILE 2
#endif
#ifdef _TC_EARLY_
#define COMPILE 4
#endif

#ifdef COMPILE
#include <portable.h>
#include "suppl.h"

#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

#ifdef _MICROC_
char *_getdcwd(int drive, char Xbuf[], unsigned length)
{	unsigned l;
	char *buf;

	DBG_ENTER("_getdcwd", Suppl_portable)
	DBG_ARGUMENTS( ("drv=%d, buf=%s, len=%u", drive, buf? "passed": "allocate", length) )

	if(length < 64) {
		eno_set(ERANGE);
		DBG_RETURN_S( 0)
	}

	if((buf = Xbuf) || (buf = eno_malloc(length))) {
		buf;						/* AX := buf */
		asm "mov si, ax";
		drive;						/* AX := drive */
		asm {
			mov dx, ax
			or ax, ax
			jz currDrive
			add al, 'A' - 1			;/* specified drive */
			jmp short putDrive
currDrive:
			mov ah, 19h
			int 21h
			add al, 'A'				;/* 0 == A: */
putDrive:
			mov Byte Ptr [si], al
			inc si
			mov Word Ptr [si], '\:'
			inc si
			inc si
			mov ah, 47h
			int 21h
			sbb ax, ax		;; AX := C flag
		}
		if(nargs()) {				/* Get path failed */
			if(!Xbuf) free(buf);
			DBG_RETURN_S( 0)
		}
	}
	DBG_RETURN_S( buf)
}

#else

#include <dir.h>		/* getdisk() */
#include <stdlib.h>
#include <string.h>
#include "suppl.h"

char *_getdcwd(int drive, char Xbuf[], unsigned length)
{	char *buf;
	IREGS r;

	DBG_ENTER("_getdcwd", Suppl_portable)
	DBG_ARGUMENTS( ("drv=%d, buf=%s, len=%u", drive, buf? "passed": "allocate", length) )

	if(length < 64) {
		eno_set(ERANGE);
		DBG_RETURN_S( 0)
	}

	chkHeap
	if((buf = Xbuf) != 0 || (buf = eno_malloc(length)) != 0) {
		chkHeap
#ifndef NDEBUG
		memset(buf, 0xAA, length);
		chkHeap
#endif
		strcpy(buf, "A:\\");
		*buf += drive? drive - 1: getdisk();
		r.r_si = FP_OFF(&buf[3]);
		r.r_dx = drive;
		r.r_ds = FP_SEG(buf);
#ifdef FEATURE_LONG_FILENAMES
		r.r_ax = 0x7147;
#else
        r.r_ax = 0x4700;
#endif
		intrpt(0x21, &r);
#ifdef FEATURE_LONG_FILENAMES
		if((r.r_flags & 1) || r.r_ax == 0x7100) {				/* Get path failed */
            r.r_ax = 0x4700;
            intrpt(0x21, &r);
#endif
            if(r.r_flags & 1) {
    			if(!Xbuf) free(buf);
    			chkHeap
    			DBG_RETURN_S( 0)
            }
#ifdef FEATURE_LONG_FILENAMES
		}
#endif
	}
	chkHeap
	DBG_RETURN_S( buf)
}

#endif

#endif	/* COMPILE */
