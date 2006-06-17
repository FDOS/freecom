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
 * header file for PSP settings

*/

#ifndef __PSP_H
#define __PSP_H

#include <portable.h>

#include <algnbyte.h>

struct PSP {
	byte psp_cpm_call0[2];		/* CP/M style program termination entry
									--> INT 20h (0xCD 0x20) */
	word psp_freeSegm;			/* segm of first free byte above process */
	byte psp_os2_fakeDOSversion;
	byte psp_cpm_call5[5];		/* FAR CALL to absolute 000C0h */
	void far*psp_oldInt22;		/* value of INT-22 restored on exit (return) */
	void far*psp_oldInt23;		/* value of INT-23 restored on exit (^Break) */
	void far*psp_oldInt24;		/* value of INT-24 restored on exit (CritErr) */
	word psp_parentPSP;
	byte psp_jft[20];
	word psp_envSegm;
	void far*psp_lastStack;		/* SS:SP on last INT-21 */
	word psp_jft_numEntries;
	byte far *psp_jft_addr;
	struct PSP far*psp_share_prevPSP;
	byte psp_dbcs_consoleFlag;
	byte psp_append_truenameFlag;
	byte psp_os2_flags;
	byte psp_netware_taskNumber;	/* valid if os2_flags == 0xCE */
	byte psp_dosVersion[2];		/* returned on DOS-30 */
	word psp_pdb_selectorNext;
	word psp_pdb_parition;
	word psp_pdb_next;
	byte psp_win_flags;		/* bit 0: set -> non-win app (==WINOLDAP) */
	byte psp_reserved1;
	byte psp_reserved1_1[2];	/* Missing from RBIL v6.1 */
	word psp_pdb_entryStack;
	byte psp_reserved2[2];
	byte psp_serviceRequest[3];	/* INT 21h / RETF */
	byte psp_reserved3[2];
	byte psp_extFCB1[7];
	byte psp_fcb1[16];
	byte psp_fcb2[16];
	byte psp_reserved4[4];
	union {
		byte psp_dta[128];
		struct {
			byte psp_cmdlineLength;
			byte psp_cmdline[127];
		};
	};
};

#include <algndflt.h>

struct SUPPL_verify_alignment_PSP {
	char x[sizeof(struct PSP) == 128 ? 1 : -1];
};

#endif
