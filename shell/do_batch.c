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

char *readbatchline(ctxtEC_t far * const ctxt)
{	ctxtEC_Batch_t far *bc;		/* current batch context */
	char *line, *p;				/* read line */
	char *name;
	FILE *f;

	dprintf(("readbatchline()\n"));
	if(doExit || doCancel || doQuit) {
		doQuit = FALSE;
		return 0;
	}

	assert(ctxt);
	assert(ctxt->ctxt_type == EC_TAG_BATCH);

	interactive = F(interactive) = 0;

	bc = ecData(ctxt, ctxtEC_Batch_t);

	assert(_fstrlen(bc->ec_fname)
	 == ctxt->ctxt_length - sizeof(ctxtEC_Batch_t));

	dprintf(("[BATCH: pos=%lu, line=%lu: %Fs]\n"
	 , bc->ec_pos, bc->ec_lnum, bc->ec_fname));

	 if((name = regStr(edupstr(bc->ec_fname))) == 0) {
	 	return 0;
	 }

	if((f = fopen(name, "rt")) == 0) {
		error_bfile_vanished(name);
		return 0;
	}

	line = 0;

	if(rewindBatchFile) {
		/* reset the statistics */
		bc->ec_lnum = 0;
		rewindBatchFile = 0;
	} else {
		if(fseek(f, bc->ec_pos, SEEK_SET))
			goto errRet;		/* silently ignore this error */
	}

	for(; !cbreak; chkp(line)) {
		if((line = Fgetline(f)) == 0) {
			error_out_of_memory();
			break;
		}
		if(!*line) {			/* End of file */
			line = 0;		/* Force ecPop() */
			break;
		}
		++bc->ec_lnum;
		if(ferror(f)) {
			error_bfile_read(name, bc->ec_lnum);
			chkPtr(line);
			StrFree(line);
			break;
		}

		assert(strlen(line) >= 1);

		/* Ignore the trailing \nonly, as some commands rely on trailing
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
		if(*p == ':' && memcmp(p, "::=", 3) != 0) {
			/* if a label is searched for test if we reached it */
			if(gotoLabel) {
				/* label: the 1st word immediately following the colon ':' */
				int len = strlen(gotoLabel);
				if(memcmpFI(p + 1, gotoLabel, len) == 0
				 && !isgraph(line[len + 1]))
					/* got it! -> proceed with next line */
					chkPtr(line);
					StrFree(gotoLabel);
			}
			continue;			/* ignore label */
		}

		if(!gotoLabel) {
			if(*p == '@') {			/* don't echo this line */
				p = ltrimcl(p + 1);
				if(!*p)				/* ignore empty lines */
					continue;
				echoBatch = 0;
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
	bc->ec_pos = ftell(f);
	fclose(f);
	return line;
}
