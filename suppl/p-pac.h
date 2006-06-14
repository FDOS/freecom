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
/* Portable include file for Pacific C */

#ifdef HI_TECH_C

#include <dos.h>
#include <stdlib.h>

	/* Pacific C does not support various stuff natively.
		Therefore we define a different macro for differing PAC
		with and without CLib.
	*/

#ifndef _PAC_
#ifndef _PAC_NOCLIB_
#ifdef _CLIB_
#define _PAC_
#else
#define _PAC_NOCLIB_
#endif	/* _CLIB_ */
#endif
#endif


#ifdef _PAC_NOCLIB_
/* Global argc/argv variables */
#define G_ARGV ___argv		/* global char **argv	variable */
#define G_ARGC ___argc		/* global int argc	variable */
	/* definition of global variables */
#define G_ARG_DEF char *G_ARGV[]; int G_ARGC;		
	/* initialization of global variables */
#define G_ARG_INIT	G_ARGV = argv; G_ARGC = argc;
extern char *G_ARGV[];		/* holds global argv */
extern int G_ARGC;			/* holds global argc */
#else
#define G_ARGV _argv		/* global char **argv	variable */
#define G_ARGC _argc		/* global int argc	variable */
#define G_ARG_DEF			/* definition of global variables */
#define G_ARG_INIT			/* initialization of global variables */
#endif



/* byte/word/dword type */
typedef unsigned char byte;
typedef unsigned word;
typedef byte FLAG;	/* PAC supports byte-aware arethmetics */
typedef byte FLAG8;
typedef unsigned long dword;

#ifdef _PAC_NOCLIB_
#define peekb(segm,ofs) (*(byte far*)MK_FP((segm),(ofs)))
#define peekw(segm,ofs) (*(word far*)MK_FP((segm),(ofs)))
#define pokeb(segm,ofs,value) (peekb((segm),(ofs)) = (byte)(value))
#define pokew(segm,ofs,value) (peekw((segm),(ofs)) = (word)(value))
#endif


#ifdef _PAC_NOCLIB_
#define _osminor (osversion() >> 8)
#define _osmajor (osversion() & 0xff)
#define _osversion osversion()
#define _psp _psp_()
extern unsigned _psp_(void);

/* get/set interrupt vector */
#include <intrpt.h>
#define set_isrfct(num,fct) setiva((num), (isr)(fct))
#define get_isr(num,var) ((var) = getiva(num))
#define set_isr(num,var) setiva((num), (var))

/* calling an interrupt */
typedef struct {
	unsigned int	r_ax;
	unsigned int	r_bx;
	unsigned int	r_cx;
	unsigned int	r_dx;
    unsigned int    r_bp;
    unsigned int    r_si;
    unsigned int    r_di;
    unsigned int    r_ds;
    unsigned int    r_es;
    unsigned int    r_flags;
} IREGS;
#define intrpt(num,regs) intr((num), (union REGPACK*)(regs))

#endif

#endif
