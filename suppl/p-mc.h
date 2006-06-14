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
/* Portable include file for Micro-C */
#ifdef _MICROC_

#if _MICROC_ -1 == -1
#error "Patch STDIO.H to add the hexadecimal version number to the macro _MICROC_, e.g. 0x136"
#endif

	/* non-predeclared global variables */
#include <external.h>
	/* struct REGPACK */
#include <intr.h>

#define iM(q)	q		/* if Micro-C */
#define nM(q)			/* not Mirco-C */
#define inM(q,w)	q	/* if-not Micro-C */
#define aF(fct) &(fct)		/* address of function */
#define aS(structure) (structure)	/* address of structure */
#define cS(strucType)		/* no structure casting */
/* The definition of TO_FP resides in <fmemory.h> */

#ifndef NULL
#define NULL 0
#endif

#define peekb peek
#define pokeb poke
#define const
#define _psp PSP

/* Global argc/argv variables */
#define G_ARGV ARGV		/* global char **argv	variable */
#define G_ARGC ARGC		/* global int argc	variable */
#define G_ARG_DEF		/* definition of global variables */
#define G_ARG_INIT		/* initialization of global variables */

/* Handling of pointers to structures */
#define sInc(poi) ((poi) += sizeof(*(poi)))
#define sDec(poi) ((poi) -= sizeof(*(poi)))
#define sIdx(array,index) ((array) + (index) * sizeof(*(array)))
#define sAdd(poi,displacement) ((poi) += (displacement) * sizeof(*(array)))
#define sSub(poi,displacement) ((poi) -= (displacement) * sizeof(*(array)))

/* byte/word/dword type */
#define byte unsigned char
#define word unsigned
#define FLAG byte	/* Micro-C has special 8-bit handling */
#define FLAG8 byte	/* dito */
#define get_ss get_ds
#define size_t unsigned

#define dword struct DWORD
struct DWORD {
	word lo;	/* offset */
	word hi;	/* segment */
};
#define setDW(var,seg,ofs) (var.hi = (seg), var.lo = (ofs))
/* Micro-C does not support (*struct_pointer).member for struct_pointer->member */
#define setDWP(varP,seg,ofs) (varP->hi = (seg), varP->lo = (ofs))
#define DW_LO(var) var.lo
#define DW_HI(var) var.hi
#define DWP_LO(varP) varP->lo
#define DWP_HI(varP) varP->hi
#define DWARG dword *			/* dword argument */

/* Simulate long constants and long arguments */
#define TO_LONG(num) 0, (unsigned)(num)
#define MK_LONG(hi,lo) (unsigned)(hi), (unsigned)(lo)
#define FARARG(var,type) word var/**/Seg, word var/**/Ofs
#define COPYFARARG(var) var/**/Seg, var/**/Ofs
#define longcmp1 longcmp

/* version variables */
#define _osmajor ((unsigned)version() >> 8)
#define _osminor (version() & 0xff)
#define _osversion version()

/* seek() whence parameters */
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

/* DOS drive handling */
#define getdisk get_drive
#define setdisk set_drive

/* set/get interrupt vector */
#define isr dword		/* type of variable to hold one vecotr */
#define set_isrfct(num,fct) set_vector((num), get_cs(), &(fct))
#define get_isr(num,var) get_vector((num), &(var).hi, &(var).lo)
#define set_isr(num,var) set_vector((num), (var).hi, (var).lo)

#define IREGS struct REGPACK
#define intrpt intr

#endif
