/*
    This file is part of SUPPL - the supplemental library for DOS
    Copyright (C) 1996-99 Steffen Kaiser

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
 *
 * All functions work over the paragraphe number of the MCB.
 * Note: Some functions use the PSP instead of the real MCB number!
 *
 * Shells are a mess in DOS. They patch their parent field to point to
 * theirselves, thus, making correct process trees impossible.
 *
 * implemented functions:
	file		function	short desription

	mcb.h		SEG2MCB		transform a memory handle into MCB number
	mcb.h		MCB2SEG		transform a MCB number into a memory handle
	mcb_apar.c	mcb_allParents	spawn function for all parents and current PSP
	mcb_env.c	isEnv		check, if MCB is an environment segment
	mcb_is.c	isMCB		check, if MCB is a valid MCB number
	mcb_leng.c	mcb_length	return the length of the *memory handle* in bytes
	mcb_mstr.c	mcb_primaryShell	return the MCB of the primary shell
	mcb_nxt.c	nxtMCB		return the MCB number of the next MCB in the chain
	mcb_owne.c	mcb_owner	return the MCB number of the owner process
	mcb_pare.c	mcb_shell	return the MCB number of the shell of mcb
	mcb_psp.c	isPSP		check, if MCB is a valid process (PSP)
	mcb_shell.c	isShell		check, if MCB is a shell process
	mcb_toen.c	mcb_toenv	transform the MCB number in the env seg, if PSP
	mcb_walk.c	mcb_walk	spawn a function for each valid MCB

*/

#ifndef __MCB_H
#define __MCB_H

#include <portable.h>

#define SEG2MCB(segm) ((segm) - 1)
/* transforms a PSP number or a memory segment address (returned by DOS
	API function 0x49) into its MCB number */

#define MCB2SEG(mcb) ((mcb) + 1)
/* opposite of SEG2MCB() */

#define SEG_OFFSET 0x10
/* Because of the segment/offset stuff of Intel chips, this is the
	displacement MCB -> SEG useable in the offset of a far address */

#ifdef _MICROC_
unsigned mcb_toenv(unsigned mcb);
/* Transform the MCB into the associated environment segment.
	The mcb must be a valid PSP. */
#else
#define mcb_toenv(mcb) peekw(mcb, SEG_OFFSET + 0x2c)
#endif

struct MCB {
	byte mcb_type;		/* type of MCB, 'Z': no following MCB, 'M': following */
	word mcb_ownerPSP;	/* PSP number of owner of this block */
	word mcb_size;		/* size of this MCB */
	byte mcb_rsrvd[3];	/* unused */
	byte mcb_name[8];	/* name of program file name if PSP created in exec() */
};
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
/* Return the length of the memory block segm (PSP) in bytes */

int mcb_walk(unsigned mcb, const MCB_WALKFUNC fct, void * const arg);
/* General purpose MCB walker.

	mcb: where to begin the walk process, if 0, begin with the 1st MCB avail
	fct: function to be called with each MCB:	(*fct)(arg, mcb)
		 The function must return 0, if the walker shall stop.
	arg: general argument to be passed unchanged to the function

	Return:	 0: end of chain reached (function never hit).
			-1: chain is corrupted.
		  else: function cancelled walk process.
*/

int isShell(const unsigned mcb);
/* Returns 1, if the MCB number (*not* PSP) is a shell;
		   0, otherwise.
   Note: The function does not check, if mcb really is a valid MCB.
*/

int isPSP(const unsigned mcb);
/* Returns 1, if the MCB number (*not* PSP) is a PSP;
		   0, otherwise.
   Note: The function does not check, if mcb really is a valid MCB.
*/

int isMCB(const unsigned mcb);
/* Returns 1, if the MCB number (*not* PSP) is valid;
		   0, otherwise.
*/

int isEnv(const unsigned mcb);
/* Returns 1, if the MCB number (*not* memory handle!) is an environment;
		   0, otherwise.
*/

unsigned nxtMCB(unsigned mcb);
/* Returns	0, if mcb is the last in the chain;
			!= 0, MCB behind this mcb.
*/

int mcb_allParents(unsigned mcb, MCB_WALKFUNC fct, void *arg);
/* Launch "(*fct)(arg, mcb)" for all parents (excluding the process mcb
	itself). The argument mcb is the MCB of the particular process.
	(*fct) must return != 0, if the walk process shall be terminated.
	The walk terminates, when a shell is reached.

	If mcb is 0, _psp is used, otherwise it is checked via "isPSP(mcb)",
	if this is (possibly) a PSP.

	Return:	  -1: MCB chain or something else corrupted (e.g. mcb was
					no PSP)
			   0: shell reached, but function didn't terminate
			else: function terminated walk process
*/

unsigned mcb_primaryShell(void);
/* There is no known way, which will never fail.

	There are three ways of locating the primary shell:
		1) Check the program INT-2E points to;
		2) Check the first shell in the MCB chain;
		3) Check the last shell that has its environment behind its PSP.

	With all the memory saving programs moving their data freely through
	the memory, all three may or may not return the correct environment.

	Therefore, all three checks are made and the result will be assumed
	as true, which comes twice.

	Return:	0:	nothing found. All free ways returned different addresses
*/

unsigned mcb_owner(unsigned mcb);
/* Return the MCB of the PSP owning mcb.
*/

unsigned mcb_shell(unsigned mcb);
/* Return the MCB of the most current launched shell of the process of mcb.
*/

#endif
