/*
	Implementation for Installable COMMAND extensions (MUX-AE)

	2000/12/10 ska
	started
*/

#include "config.h"

#ifdef FEATURE_INSTALLABLE_COMMANDS

#include <assert.h>
#include <ctype.h>
#include <dos.h>
#include <string.h>

#include "command.h"
#include "crossjmp.h"
#include "mux_ae.h"

#if BUFFER_SIZE_MUX_AE > 255
#error "BUFFER_SIZE_MUX_AE must not exceed 255"
#endif

/* Check for extensions and perform it, on success return != 0 */
/* Also: The contents of both buffers is monitored in order to
	check if they got overflowed during the MUX call. If so,
	the executation is aborted. */
int runExtension(char * const command, char * const line)
{	int clen, llen;
	struct REGPACK r;
	char *p;

	assert(command);
	assert(line);

	assert(strlen(command) < BUFFER_SIZE_MUX_AE);
	assert(strlen(line) <= BUFFER_SIZE_MUX_AE);

	assert(FP_SEG(command) == FP_SEG(line));

	/* Fill the length bytes */
	command[-1] = (char)(clen = strlen(command));
	line[-1]    = (char)(llen = strlen(line));
	command[-2] = line[-2] = (char)BUFFER_SIZE_MUX_AE;

	/* 4dos v4 compatible space padding */
	memset(command + clen, ' ', BUFFER_SIZE_MUX_AE - clen);
	/* The command line is \xd terminated, for savety reasons an \0 is
		added too */
	strcat(line + llen, "\xd");
	line[BUFFER_SIZE_MUX_AE] = '\0';

/* Both strings have been prepared now; the MUX call is going to happen */
	r.r_ax = 0xae00;		/* Installable Commands check for extension */
	r.r_dx = 0xffff;		/* Magic value */
	r.r_cx = -llen;			/* length of command line tail (4dos v4) */
	r.r_ds = r.r_es = FP_SEG(command);
	r.r_bx = FP_OFF(line) - 2;
	r.r_si = FP_OFF(command) - 1;
	r.r_di = 0;				/* Magic value 4dos v4 */

	intr(0x2f, &r);

	switch(r.r_ax & 0xFF) {
	case 0x00:		/* No appropriate extension found */
		break;

	default:		/* Invalid response */
		dprintf( ("[Invalid response from Installable Commands handler: 0x%02x]\n", r.r_ax & 0xFF) );
		break;

	case 0xFF:		/* Is an extension -> execute the Installable Command */
		r.r_ax = 0xae01;
		intr(0x2f, &r);
		if(command[-1] == 0)		/* The command had been processed */
			return 1;				/* Stop interpreting the command */

		break;
	}

/* Cleanup: Adjust buffers and check for overflow */
	if((unsigned char)line[-2] != BUFFER_SIZE_MUX_AE
#if BUFFER_SIZE_MUX_AE < 255
	 || (unsigned char)line[-1] > BUFFER_SIZE_MUX_AE
#endif
	 || line[BUFFER_SIZE_MUX_AE]) {
		/* Yiek! That looks very much like an overflow!! */
		dprintf( ("[Memory corrupted during Installable Commands handler]\n") );
		longjmp(jmp_beginning, E_CorruptMemory);
	}

	/* Check command and transform it back into C-style string */
	p = command + command[-1];
	while(--p >= command && isspace(*p));
	p[1] = 0;

	/* Check the command line and transform it into a C-style string */
	/* Must terminate as line[BUFFER_SIZE] == 0 */
	for(p = line; *p && *p != '\xd'; ++p);
	*p = 0;

	return 0;			/* Proceed command processing as usual */
}

#endif
