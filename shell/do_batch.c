/*
 * Read and return the next executable line from the current batch file
 *
 * If no batch file is current or no further executable lines are found
 * return NULL.
 *
 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <dfn.h>
#include <dynstr.h>
#include <supplio.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/cmdline.h"
#include "../err_fcts.h"

#ifdef DEBUG
static int chkp(char *p)
{	chkPtr(p);
	StrFree(p);
	return 1;
}
#else
#define chkp(p)	StrFree((p))
#endif

char *readBatch(char far * const ctxt)
{	unsigned long pos, lnr;
	char *line, *p;				/* read line */
	char *name;
	FILE *f;
	unsigned idFnam;
	char buf[EC_LENGTH_B];

	dprintf(("readBatch()\n"));
	if(lflag_doExit || lflag_doCancel || lflag_doQuit) {
		dprintf(("[BATCH: Exit, Cancel, or Quit terminated script]\n"));
		lflag_doQuit = FALSE;
		return 0;
	}

	assert(ctxt);
	assert(ctxt[0] == EC_TAG_BATCH);

	lflag_interactive = gflag_interactive = 0;

	if(ecScanArg(ctxt, 3, "%u|%lu %lu", &idFnam, &pos, &lnr) != E_None
	 || (name = ecString(idFnam)) == 0)
		return 0;

	dprintf(("[BATCH: pos=%lu, line=%lu: %s]\n", pos, lnr, name));
#ifdef DEBUG
	if(lflag_gotoLabel)
		dprintf(("[BATCH: Searching for label: %s]\n", lflag_gotoLabel));
#endif

	if((f = fopen(name, "rt")) == 0) {
		error_bfile_vanished(name);
		return 0;
	}

	line = 0;

	if(lflag_rewindBatchFile) {
		/* reset the statistics */
		lnr = 0;
		lflag_rewindBatchFile = 0;
		dprintf(("[BATCH: Script rewinded]\n"));
	} else {
		if(fseek(f, pos, SEEK_SET))
			goto errRet;		/* silently ignore this error */
								/* as line == 0 --> gets popped */
	}

	for(; !cbreak; chkp(line)) {
		chkRegStr(name);
		dprintf(("[BATCH: about to read line #%lu from @%lu]\n"
		 , lnr + 1, ftell(f)));
		if((line = Fgetline(f)) == 0) {
			error_out_of_memory();
			break;
		}
		if(!*line) {			/* End of file */
			dprintf(("[BATCH: End of file reached]\n"));
			myfree(line);
			line = 0;		/* Force ecPop() */
			break;
		}
		++lnr;
		if(ferror(f)) {
			error_bfile_read(name, lnr);
			myfree(line);
			line = 0;
			break;
		}

		assert(strlen(line) >= 1);

		/* Ignore the trailing \n only, as some commands rely on trailing
			whitespaces. */
		assert(strchr(line, '\0')[-1] == '\n');
		strchr(line, '\0')[-1] = 0;
		/* Strip leading whitespace */
		p = ltrimcl(line);

		assert(p);

		/* ignore empty lines */
		if(!*p)
			continue;

			/* ::= is a special internal quotation mechanism */
		if(*p == ':' && memcmp(p, FORCE_INTERNAL_COMMAND_STRING
		 , (sizeof(FORCE_INTERNAL_COMMAND_STRING)-1)) != 0) {
			/* if a label is searched for test if we reached it */
			if(lflag_gotoLabel) {
				/* label: the 1st word immediately following the colon ':' */
				int len = strlen(lflag_gotoLabel);
				if(memcmpFI(p + 1, lflag_gotoLabel, len) == 0
				 && !isgraph(line[len + 1]))
					/* got it! -> proceed with next line */
					chkPtr(lflag_gotoLabel);
					StrFree(lflag_gotoLabel);
			}
			dprintf(("BATCH: Skipping label: %s]\n", line));
			continue;			/* ignore label */
		}

		if(!lflag_gotoLabel) {
			if(*p == '@') {			/* don't echo this line */
				p = ltrimcl(p + 1);
				if(!*p)				/* ignore empty lines */
					continue;
				lflag_echo = 0;
				dprintf(("[CTXT: disable local ECHO status]\n"));
			}

			/* assume that clearing the space stepped over is
				less _resource_ consuming than memmove()'ing.
				We also waste one byte for the overwritten '\n'. */
			if(p != line)
				memset(line, ' ', p - line);
			break;
		}
		/* else: continue until label is found or EOF */
	}

errRet:
	/* Record the current position */
	assert(_fstrchr(ctxt, '|'));
	sprintf(buf, "|%lu %lu|", ftell(f), lnr);
		/* The \0 byte is not transferred */
	_fmemcpy(_fstrchr(ctxt, '|'), TO_FP(buf), strlen(buf));
	fclose(f);
	return line;
}
