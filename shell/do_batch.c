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
//#include <stdlib.h>
#include <string.h>

#include <dfn.h>
#include <dynstr.h>
#include <supplio.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/cmdline.h"
#include "../err_fcts.h"

char *readbatchline(ctxtEC_t far * const ctxt)
{	ctxtEC_Batch_t far *bc;		/* current batch context */
	char *line, *p;				/* read line */
	char *fnam;
	FILE *f;

	dprintf(("readbatchline()\n"));
	if(doExit || doCancel || doQuit) {
		doQuit = FALSE;
		return 0;
	}

	assert(ctxt);
	assert(ctxt->ctxt_type == EC_TAG_BATCH);

	bc = ecData(ctxt, ctxtEC_Batch_t);

	assert(_fstrlen(bc->ec_fname)
	 == ctxt->ctxt_length - sizeof(ctxtEC_Batch_t));

	dprintf(("[BATCH: pos=%ul, line=%ul: %Fs]\n"
	 , bc->ec_pos, bc->ec_lnum, bc->ec_fname));

	 if((fnam = regStr(edupstr(bc->ec_fname))) == 0) {
	 	return 0;
	 }

	if((f = fopen(fnam, "rb")) == 0) {
		error_bfile_vanished(fnam);
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

	for(; !chkCBreak(BREAK_BATCHFILE); StrFree(line)) {
		if((line = Fgetline(f)) == 0) {
			if(errno == ENOMEM)
				error_out_of_memory();
			break;		/* end of file */
		}
		++bc->ec_lnum;
		if(ferror(f)) {
			error_bfile_read(fnam, bc->ec_lnum);
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
				if(memicmp(p, gotoLabel, len) == 0 && !isgraph(line[len]))
					/* got it! -> proceed with next line */
					StrFree(gotoLabel);
			}
			continue;			/* ignore label */
		}

		if(!gotoLabel) {
			if(*p == '@') {			/* don't echo this line */
				p = ltrimcl(p + 1);
				if(!*p)				/* ignore empty lines */
					continue;
				echoBatch = 1;
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
