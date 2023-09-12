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
/* Portable include file for GCC (ia16-elf-gcc) */

#ifdef __GNUC__

#include <stdlib.h>
#include <stddef.h>
#ifndef EZERO
#define EZERO 0
#endif

#ifdef __FAR
#define far __far
#ifndef __IA16_FEATURE_FAR_STATIC_STORAGE
#error Please upgrade ia16-elf-gcc to get support for far static storage
#endif
#else
#define far
#endif

#define cdecl __attribute__((cdecl))

/* Global argc/argv variables */
#define G_ARGV _argv		/* global char **argv	variable */
#define G_ARGC _argc		/* global int argc	variable */
#define G_ARG_DEF			/* definition of global variables */
#define G_ARG_INIT			/* initialization of global variables */



/* byte/word/dword type */
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned FLAG;
typedef unsigned FLAG8;
#ifdef __LP64__
typedef unsigned dword;
#else
typedef unsigned long dword;
#endif

#ifndef _CLIB_
#define peekb(segm,ofs) (*(byte far*)MK_FP((segm),(ofs)))
#define peekw(segm,ofs) (*(word far*)MK_FP((segm),(ofs)))
#define pokeb(segm,ofs,value) (peekb((segm),(ofs)) = (byte)(value))
#define pokew(segm,ofs,value) (peekw((segm),(ofs)) = (word)(value))
#endif


#define _osversion MK_OSVERS(_osmajor, _osminor)

/* get/set interrupt vector */
typedef void __far *isr;
#define set_isrfct(num,fct) _dos_setvect((num), (isr)(fct))
#define get_isr(num,var) ((var) = _dos_getvect(num))
#define set_isr(num,var) _dos_setvect((num), (var))
#define ASMINTERRUPT

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
#define intrpt(num,regs) _intrf((num), (union REGPACK*)(regs))

static inline unsigned short CS_(void)
{
	unsigned short ret;
	asm volatile("mov %%cs, %0" : "=rm"(ret));
	return ret;
}
#define _CS CS_()

/* get/set current working drive */
static inline int getdisk(void)
{
	unsigned char ret;
	asm volatile("int $0x21" : "=Ral"(ret) : "Rah"((char)0x19));
	return ret;
}

static inline int setdisk(int drive)
{
	unsigned char ret;
	asm volatile("int $0x21" : "=Ral"(ret) : "Rah"((char)0xe), "d"(drive));
	return ret;
}

/* get/set file time and date */
static inline int _dos_getftime(int fd, unsigned *date, unsigned *time)
{
	unsigned int ret, carry;

	asm volatile("int $0x21; sbb %0, %0" :
		     "=r"(carry), "=a"(ret), "=c"(*time), "=d"(*date) :
		     "a"(0x5700), "b"(fd));
	return carry ? ret : 0;
}

static inline int _dos_setftime(int fd, unsigned date, unsigned time)
{
	unsigned int ret, carry;

	asm volatile("int $0x21; sbb %0, %0" :
		     "=r"(carry), "=a"(ret) :
		     "a"(0x5701), "b"(fd), "c"(time), "d"(date));
	return carry ? ret : 0;
}

static inline void far *_dos_getvect(int intno)
{
	unsigned short seg, off;
	asm volatile("int $0x21" :
		     "=e"(seg), "=b"(off) :
		     "Rah"((char)0x35), "Ral"((char)intno));
	return MK_FP(seg,off);
}

static inline void _dos_setvect(int intno, void far *vect)
{
	asm volatile("int $0x21" :
		     : "Rah"((char)0x25), "Ral"((char)intno),
		       "Rds"(FP_SEG(vect)), "d"(FP_OFF(vect)));
}

static inline void far *getdta(void)
{
	unsigned short seg, off;
	asm volatile("int $0x21" : "=e"(seg), "=b"(off) : "Rah"((char)0x2f));
	return MK_FP(seg,off);
}

static inline void setdta(void far *dta)
{
	asm volatile("int $0x21" :
		     : "Rah"((char)0x1a), "Rds"(FP_SEG(dta)), "d"(FP_OFF(dta)));
}
#endif
