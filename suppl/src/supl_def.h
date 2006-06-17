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
#ifndef __SUPPL_COMPILER_DEFINES
#define __SUPPL_COMPILER_DEFINES

/**********************************
 ***** Pacific HiTech C
 **********************************/
#ifdef HI_TECH_C
	/* Probe if PAC is used in conjunction with CLib */
#include <stddef.h>

#ifdef _CLIB_
#define _PAC_	1
#else
#define _PAC_NOCLIB_	1
#endif	/* _CLIB_ */

#endif /* HI_TECH_C */


/**********************************
 ***** Early Turbo C++
 **********************************/
#ifdef __TURBOC__
#if __TURBOC__ <= 0x297
#define _TC_EARLY_
#else
#define _TC_LATER_
#endif
#endif

#endif
