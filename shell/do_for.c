/* $id$

	Perform a F context (pattern, varname, cmd)

	The first argument of 'pattern' is extracted, then, if findfirst()
	succeeds on it, a f context is created.
	In both cases the cmd is expanded with the pattern or the found
	file and returned.

	Return:
		0: on error
*/

#include "../config.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <dfn.h>
#include <dynstr.h>
#include <fmemory.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../include/misc.h"
#include "../err_fcts.h"

/* Patch the FOR command line with the currently found argument
	and return a dynamically allocated command line.
	param is to be deallocated */
static char *forPatchCmdline(const char far* const varname
	, const char far* const cmd
	, char *param)
{	char *p;
	int rv;

	if((p = edupstr(varname)) == 0) {
		myfree(param);
		return 0;
	}

	/* Record the value of the variable */
	if(*p == '%')		/* hackery variable */
		/* Add its value to the internal variables */
		rv = ctxtSetS(CTXT_TAG_IVAR, p, param);
	else
		rv = chgEnv(p, param);

	myfree(p);
	myfree(param);

	return rv == E_None? edupstr(cmd): 0;
}

char *readFORfirst(ctxtEC_t far * const ctxt)
{	char far* pattern, far* varname, far* cmd;	/* param of ctxt */
	struct ffblk f;
	char far* next;			/* poi into pattern where to set start of
								context to */

	char *param;				/* current argument */

	lflag_doQuit = cbreak;

	dprintf(("readFORfirst()\n"));
	if(lflag_doExit || lflag_doCancel || lflag_doQuit) {
		dprintf(("[FOR#1: Exit, Cancel, or Quit terminated script]\n"));
		return 0;
	}

	assert(ctxt);
	assert(ctxt->ctxt_type == EC_TAG_FOR_FIRST);
	assert(ctxt->ctxt_length >= 3);

	pattern = ecData(ctxt, char);
	assert(_fstrlen(pattern) < ctxt->ctxt_length - 3);

	varname = pattern + _fstrlen(pattern) + 1;
	assert(_fstrlen(pattern) + _fstrlen(varname) < ctxt->ctxt_length - 3);

	cmd = varname + _fstrlen(varname) + 1;
	assert(_fstrlen(pattern) + _fstrlen(varname) + _fstrlen(cmd)
	 == ctxt->ctxt_length - 3);

	dprintf(("[FOR#1: %Fs (%Fs) %Fs]\n", varname, pattern, cmd));

	if(!*pattern)						/* nothing to do */
		return 0;

	if((next = _fstrchr(pattern, ES_STRING_DELIM)) == 0) {
		/* This is the last pattern */
		param = edupstr(pattern);
		next = pattern + strlen(param);	/* '\0' byte */
	} else {
		*next = '\0';
		param = edupstr(pattern);
		++next;
	}

	if(!param) {
		return 0;
	}

	if(ecShrink(next - pattern) != E_None)
		return 0;

	esDecode(param);			/* decode string from internal quote form */

	if(findfirst(param, &f, FA_NORMAL) == 0) {	/* need a f-context */
		*dfnfilename(param) = '\0';	/* extract path */
		if(ecMkf(&f, varname, cmd, param) != E_None)
			return 0;
		chkPtr(param);
		if(!StrCat(param, f.ff_name)) {
			myfree(param);
			error_out_of_memory();
			return 0;
		}
	}

	return forPatchCmdline(varname, cmd, param);
}

char *readFORnext(ctxtEC_t far * const ctxt)
{	char far* varname, far* cmd;	/* param of ctxt */
	ctxtEC_For_t far*fc;
	struct ffblk f;

	char *param;				/* current argument */

	lflag_doQuit = cbreak;

	dprintf(("readFORnext()\n"));
	if(lflag_doExit || lflag_doCancel || lflag_doQuit) {
		dprintf(("[FOR#2: Exit, Cancel, or Quit terminated script]\n"));
		return 0;
	}

	assert(ctxt);
	assert(ctxt->ctxt_type == EC_TAG_FOR_NEXT);
	assert(ctxt->ctxt_length >= sizeof(ctxtEC_For_t));

	fc = ecData(ctxt, ctxtEC_For_t);
	assert(_fstrlen(fc->ec_prefix)
	 == ctxt->ctxt_length - sizeof(ctxtEC_For_t));

	assert(ctxtp->ctxt_size < 0x1000);
	varname = MK_FP(ctxtMain, fc->ec_varname);
	cmd = MK_FP(ctxtMain, fc->ec_cmd);
	assert(cmd[-1] == 0
	 && _fnormalize(cmd) == _fnormalize(varname + _fstrlen(varname) + 1));

		/* necessary information to let findnext() do some good */
	_fmemcpy(TO_FP(&f), fc->ec_ffblk, sizeof(fc->ec_ffblk));
	if(findnext(&f) != 0) {		/* f-context used up */
		dprintf(("[FOR#2: No further match]\n"));
		return 0;
	}

	param = _fdupstr(fc->ec_prefix);
	chkPtr(param);
	if(!param || !StrCat(param, f.ff_name)) {
		myfree(param);
		error_out_of_memory();
		return 0;
	}

		/* save the info for next run */
	_fmemcpy(fc->ec_ffblk, TO_FP(&f), sizeof(fc->ec_ffblk));
	dprintf(("[FOR#2: %Fs (%s) %Fs]\n", varname, param, cmd));

	return forPatchCmdline(varname, cmd, param);
}
