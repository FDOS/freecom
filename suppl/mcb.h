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

word mcb_env(const word segm);
#ifdef NDEBUG
#define mcb_env(mcb) peekw((mcb), SEG_OFFSET + 0x2c)
#endif

#include <algnbyte.h>
struct MCB {
	byte mcb_type;		/* type of MCB, 'Z': no following MCB, 'M': following */
	word mcb_ownerPSP;	/* PSP number of owner of this block */
	word mcb_size;		/* size of this MCB */
	byte mcb_rsrvd[3];	/* unused */
	byte mcb_name[8];	/* name of program file name if PSP created in exec() */
};
#include <algndflt.h>
struct SUPPL_verify_alignment_MCB {
	char x[ 1 ];
};
	/* Don't use these constant; use offsetof(struct MCB, xxx) instead */
#define MCB_OFF_TYPE 0
#define MCB_OFF_OWNER 1
#define MCB_OFF_SIZE 3
#define MCB_OFF_NAME 8

#ifdef _MICROC_
#define MCB_WALKFUNC int *
#else
typedef int (*MCB_WALKFUNC)(void *, word);
#endif

unsigned mcb_length(const word segm);
int mcb_walk(word mcb, const MCB_WALKFUNC fct, void * const arg);
int isShell(const word mcb);
int isPSP(const word mcb);
int isMCB(const word mcb);
int isEnv(const word mcb);
word nxtMCB(word mcb);
int mcb_allParents(word mcb, MCB_WALKFUNC fct, void *arg);
word mcb_primaryShell(void);
word mcb_owner(word mcb);
word mcb_shell(word mcb);
word mcb_first(void);

#endif
