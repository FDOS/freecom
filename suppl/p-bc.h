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
/* Portable include file for Turbo/Borland C */
#ifdef __TURBOC__


/* Global argc/argv variables */
#define G_ARGV _argv		/* global char **argv	variable */
#define G_ARGC _argc		/* global int argc	variable */
#define G_ARG_DEF			/* definition of global variables */
#define G_ARG_INIT			/* initialization of global variables */

/* byte/word/dword type */
typedef unsigned char byte;
typedef unsigned short word;
typedef int FLAG;	/* no special 8-bit handling */
typedef unsigned FLAG8;	/* no special 8-bit handling */
typedef unsigned long dword;

/* peekb() && pokeb() exist */
#define peekw peek
#define pokew poke

#define _osversion MK_OSVERS(_osmajor, _osminor)

/* get/set interrupt vecotr */
typedef void interrupt (far *isr)();
#define set_isrfct(num,fct) setvect((num), (isr)(fct))
#define get_isr(num,var) ((var) = getvect(num))
#define set_isr(num,var) setvect((num), (var))

/* calling interrupt */
typedef struct REGPACK IREGS;
#define intrpt intr


#endif
