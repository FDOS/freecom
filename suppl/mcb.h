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
/* $Id$
 * $Locker$	$Name$	$State$
 *
 * header file for the MCB handling functions

*/

#ifndef __MCB_H
#define __MCB_H

#include <portable.h>

#define SEG2MCB(segm) ((segm) - 1)
#define MCB2SEG(mcb) ((mcb) + 1)

	/* Don't use this constant; use SEG2MCB() or MCB2SEG() instead */
#define SEG_OFFSET 0x10

unsigned mcb_env(const unsigned segm);
#ifdef NDEBUG
#define mcb_env(mcb) peekw((mcb), SEG_OFFSET + 0x2c)
#endif

struct MCB {
	byte mcb_type;		/* type of MCB, 'Z': no following MCB, 'M': following */
	word mcb_ownerPSP;	/* PSP number of owner of this block */
	word mcb_size;		/* size of this MCB */
	byte mcb_rsrvd[3];	/* unused */
	byte mcb_name[8];	/* name of program file name if PSP created in exec() */
};
	/* Don't use these constant; use offsetof(struct MCB, xxx) instead */
#define OFF_MCB_TYPE 0
#define OFF_MCB_OWNER 1
#define OFF_MCB_SIZE 3
#define OFF_MCB_NAME 8

#ifdef _MICROC_
#define MCB_WALKFUNC int *
#else
typedef int (*MCB_WALKFUNC)(void *, unsigned);
#endif

unsigned mcb_length(const unsigned segm);
int mcb_walk(unsigned mcb, const MCB_WALKFUNC fct, void * const arg);
int isShell(const unsigned mcb);
int isPSP(const unsigned mcb);
int isMCB(const unsigned mcb);
unsigned isEnv(const unsigned mcb);
unsigned nxtMCB(unsigned mcb);
int mcb_allParents(unsigned mcb, MCB_WALKFUNC fct, void *arg);
unsigned mcb_primaryShell(void);
unsigned mcb_owner(unsigned mcb);
unsigned mcb_shell(unsigned mcb);

#endif
