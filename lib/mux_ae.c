/*	$id$
	$Locker$	$Name$	$State$

	Implementation for Installable COMMAND extensions (MUX-AE)

	Check for extensions and perform it, on success return != 0 

	This file bases on MUX_AE.C of FreeCOM v0.81 beta 1.

	$Log$
	Revision 1.1.4.1  2001/06/19 20:42:23  skaus
	Update #1

	Revision 1.1  2001/04/12 00:33:53  skaus
	chg: new structure
	chg: If DEBUG enabled, no available commands are displayed on startup
	fix: PTCHSIZE also patches min extra size to force to have this amount
	   of memory available on start
	bugfix: CALL doesn't reset options
	add: PTCHSIZE to patch heap size
	add: VSPAWN, /SWAP switch, .SWP resource handling
	bugfix: COMMAND.COM A:\
	bugfix: CALL: if swapOnExec == ERROR, no change of swapOnExec allowed
	add: command MEMORY
	bugfix: runExtension(): destroys command[-2]
	add: clean.bat
	add: localized CRITER strings
	chg: use LNG files for hard-coded strings (hangForEver(), init.c)
		via STRINGS.LIB
	add: DEL.C, COPY.C, CBREAK.C: STRINGS-based prompts
	add: fixstrs.c: prompts & symbolic keys
	add: fixstrs.c: backslash escape sequences
	add: version IDs to DEFAULT.LNG and validation to FIXSTRS.C
	chg: splitted code apart into LIB\*.c and CMD\*.c
	bugfix: IF is now using error system & STRINGS to report errors
	add: CALL: /N
	
 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>

#include <nls_f.h>
#include <dynstr.h>

#include "../include/command.h"
#include "../include/crossjmp.h"
#include "../err_fcts.h"
#include "../include/mux_ae.h"

#if BUFFER_SIZE_MUX_AE > 255
#error "BUFFER_SIZE_MUX_AE must not exceed 255"
#endif

/* Also: The contents of both buffers is monitored in order to
	check if they got overflowed during the MUX call. If so,
	the executation is aborted. */
int runExtension(char ** const command
	, const char * const line
	, char ** const newargs)
{	int clen, llen, xlen, nlen;
	struct REGPACK r, r2;
	char *muxCmd, *muxArg, *muxBuf, *p;

	assert(command);
	assert(*command);
	assert(line);
	assert(newargs);

	if((clen = strlen(*command)) >= BUFFER_SIZE_MUX_AE
	 || (llen = strlen(line)) >= BUFFER_SIZE_MUX_AE
	 || llen + clen >= BUFFER_SIZE_MUX_AE) {
		error_long_mux_line();
		return 0;
	}

	/* Duplicate & fill in the length bytes. */
		/* Both buffers must be located in the same segment */
	if((muxBuf = emalloc(2 * (BUFFER_SIZE_MUX_AE + 4))) == 0)
	 	return 0;

		/* fill everything with magic values */
	memset(muxBuf, (char)BUFFER_SIZE_MUX_AE, 2 * (BUFFER_SIZE_MUX_AE + 4));
	muxCmd = muxBuf + 2;
	muxArg = muxBuf + BUFFER_SIZE_MUX_AE + 4 + 2;
	memcpy(muxCmd, *command, clen);
	muxCmd[-1] = (char)clen;
	memcpy(muxArg, muxCmd, clen);
	memcpy(muxArg + clen, line, llen);
	muxArg[-1] = (char)(xlen = llen + clen);
/*	muxCmd[-2] = muxArg[-2] = (char)BUFFER_SIZE_MUX_AE;
		see above memset() */

	assert(xlen <= 255);

	/* 4dos v4 compatible space padding */
	memset(muxCmd + clen, ' ', BUFFER_SIZE_MUX_AE - clen);
	/* The command line is \xd terminated, for savety reasons an \0 is
		added too */
	strcpy(muxArg + xlen, "\xd");
	muxCmd[BUFFER_SIZE_MUX_AE]		/* Make sure the buffers is terminated */
	 = muxArg[BUFFER_SIZE_MUX_AE] = '\0';

/* Both strings have been prepared now; the MUX call is going to happen */
	r.r_ax = 0xae00;		/* Installable Commands check for extension */
	r.r_dx = 0xffff;		/* Magic value */
	r.r_cx = xlen | 0xff00;	/* length of command line tail (4dos v4) */
	r.r_ds = r.r_es = FP_SEG(muxBuf);
	r.r_bx = FP_OFF(muxArg);
	r.r_si = FP_OFF(muxCmd);
	r.r_di = 0;				/* Magic value 4dos v4 */
	memcpy(&r2, &r, sizeof(r2));
	intr(0x2f, &r);

	if((byte)muxCmd[-2] != BUFFER_SIZE_MUX_AE
#if BUFFER_SIZE_MUX_AE < 255
	 || (byte)muxCmd[-1] > BUFFER_SIZE_MUX_AE
#endif
	 || muxCmd[BUFFER_SIZE_MUX_AE]
	 || (byte)muxArg[-2] != BUFFER_SIZE_MUX_AE
#if BUFFER_SIZE_MUX_AE < 255
	 || (byte)muxArg[-1] > BUFFER_SIZE_MUX_AE
#endif
	 || muxArg[BUFFER_SIZE_MUX_AE]) {
		/* Yiek! That looks very much like an overflow!! */
		dprintf( ("[Memory corrupted during Installable Commands handler]\n") );
		longjmp(jmp_beginning, E_CorruptMemory);
	}


	switch(r.r_ax & 0xFF) {
	case 0x00:		/* No appropriate extension found */
		break;

	default:		/* Invalid response */
		dprintf( ("[Invalid response from Installable Commands handler: 0x%02x]\n", r.r_ax & 0xFF) );
		break;

	case 0xFF:		/* Is an extension -> execute the Installable Command */
		r2.r_ax = 0xae01;
		r2.r_cx = clen;
		intr(0x2f, &r2);

		if((byte)muxCmd[-2] != BUFFER_SIZE_MUX_AE
#if BUFFER_SIZE_MUX_AE < 255
		 || (byte)muxCmd[-1] > BUFFER_SIZE_MUX_AE
#endif
		 || muxCmd[BUFFER_SIZE_MUX_AE]
		 || (byte)muxArg[-2] != BUFFER_SIZE_MUX_AE
#if BUFFER_SIZE_MUX_AE < 255
		 || (byte)muxArg[-1] > BUFFER_SIZE_MUX_AE
#endif
		 || muxArg[BUFFER_SIZE_MUX_AE]) {
			/* Yiek! That looks very much like an overflow!! */
			dprintf( ("[Memory corrupted during Installable Commands handler]\n") );
			longjmp(jmp_beginning, E_CorruptMemory);
		}

		if(muxCmd[-1] == 0) {	/* The command had been processed */
			free(muxBuf);
			return 1;			/* Stop interpreting the command */
		}

		break;
	}

/* Cleanup: Adjust buffers and check for overflow */
	/* Check command and transform it back into C-style string */
	p = muxCmd + (byte)muxCmd[-1];
	while(--p >= muxCmd && isspace(*p));
	*++p = 0;
	if(*muxCmd) {
		if((p = erealloc(*command, (nlen = p - muxCmd) + 1)) == 0) {
			free(muxBuf);
			return 0;
		}
		StrFUpr(muxCmd);		/* make sure it's still uppercased */
		memcpy(*command = p, muxCmd, nlen + 1);
	} else {
		StrFree(*command);
		nlen = 0;
	}

	/* Check the command line and transform it into a C-style string */
	/* Must terminate as line[BUFFER_SIZE] == 0 */
	p = muxArg + (byte)muxArg[-1];
	while(--p >= muxArg && *p == '\xd');
	*++p = 0;
	if((p - muxArg) >= nlen
	 && ((p - muxArg) - nlen != llen || strcmp(&muxArg[nlen], line) != 0)) {
	 	/* new arguments */
		/* Should never trigger, because the buffer for the command
			has the same size as the buffer for the argument itself.
			Because of the spurious length bytes:
				&muxArg[nlen] - muxBuf > BUFFER_SIZE_MUX_AE
			and because:
				strlen(&muxArg[nlen]) <= BUFFER_SIZE_MUX_AE
			both memory areas cannot overlap. */
		assert(&muxArg[nlen] - muxBuf > BUFFER_SIZE_MUX_AE);
		assert(strlen(&muxArg[nlen]) < BUFFER_SIZE_MUX_AE);
		strcpy(muxBuf, &muxArg[nlen]);
		*newargs = StrTrim(muxBuf);
		return 0;
	}

	free(muxBuf);
	return 0;			/* Proceed command processing as usual */
}
