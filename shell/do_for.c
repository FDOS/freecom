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
#include <dir.h>
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
static char *forPatchCmdline(const unsigned idVarname
	, const unsigned idCmd
	, char *param)
{	char *p;
	int rv;

		/* Duplicate the name as it may be alterred when the
			environment gets modified as the name is located in the
			env as well */
	if((p = estrdup(ecString(idVarname))) != 0) {
		/* Record the value of the variable */
		if(*p == '%')		/* hackery variable */
			/* Add its value to the internal variables */
			rv = ctxtSetS(CTXT_TAG_IVAR, p, param);
		else
			rv = chgEnv(p, param);
		myfree(p);
	} else
		rv = E_NoMem;

	myfree(param);

	return rv == E_None? estrdup(ecString(idCmd)): 0;
}

char *readFORfirst(char * const ctxt)
{	unsigned idPattern, idVarname, idCmd;
	char *pattern;
	struct ffblk f;
	char buf[EC_LENGTH_F];

	lflag_doQuit = cbreak;

	dprintf(("readFORfirst()\n"));
	if(lflag_doExit || lflag_doCancel || lflag_doQuit) {
		dprintf(("[FOR#1: Exit, Cancel, or Quit terminated script]\n"));
		return 0;
	}

	assert(ctxt);
	assert(*ctxt == EC_TAG_FOR_FIRST);

	if(ecScanArg(ctxt, 3, "%u %u|%u", &idVarname, &idCmd, &idPattern) != E_None
	 || idPattern <= idCmd			/* no more patterns */
	 || (pattern = ecString(idPattern)) == 0)
		return 0;

#ifdef DEBUG
	{ char *varname, *cmd;
		if((varname = ecString(idVarname)) == 0
		 || (cmd = ecString(idCmd)) == 0)
		 	return 0;
		dprintf(("[FOR#1: %Fs (%Fs) %Fs]\n", varname, pattern, cmd));
	}
#endif

	/* modify the context string to hold the id of the next pattern */
	assert(idPattern > 0);
	assert(strchr(ctxt, '|'));
	sprintf(buf, "|%u|", idPattern - 1);
		/* don't transfer the \0 */
	memcpy(strchr(ctxt, '|'), buf, strlen(buf));

	if(findfirst(pattern, &f, 0) == 0) {	/* need a f-context */
		*dfnfilename(pattern) = '\0';	/* extract path */
		if(ecMkf(&f, idVarname, idCmd, pattern) != E_None)
			return 0;
		if((pattern = StrConcat(2, pattern, f.ff_name)) == 0) {
			error_out_of_memory();
			return 0;
		}
		chkHeap
	}

	return forPatchCmdline(idVarname, idCmd, pattern);
}

char *readFORnext(char far * const ctxt)
{	unsigned idFfblk, idPrefix, idVarname, idCmd;
	char *prefix, *ffstr;
	struct ffblk f;

	lflag_doQuit = cbreak;

	dprintf(("readFORnext()\n"));
	if(lflag_doExit || lflag_doCancel || lflag_doQuit) {
		dprintf(("[FOR#2: Exit, Cancel, or Quit terminated script]\n"));
		return 0;
	}

	assert(ctxt);
	assert(*ctxt == EC_TAG_FOR_NEXT);

	prefix = 0;
	if(ecScanArg(ctxt, 4, "%u %u %u %u"
	  , &idFfblk, &idVarname, &idCmd, &idPrefix) != E_None
	 || (idPrefix && (prefix = ecString(idPrefix)) == 0)
	 || (ffstr = ecString(idFfblk)) == 0)
		return 0;

		/* necessary information to let findnext() do some good */
	esDecode(&f, ffstr, 21);
	if(findnext(&f) != 0) {		/* f-context used up */
		dprintf(("[FOR#2: No further match]\n"));
		return 0;
	}

	if((prefix = StrConcat(2, prefix, f.ff_name)) == 0) {
		error_out_of_memory();
		return 0;
	}

			/* 21: length of ffblk structure required for findnext() */
	if((ffstr = regStr(esEncMem(&f, 21))) == 0) {
		error_out_of_memory();
		return 0;
	}
	if(ctxtSet(CTXT_TAG_STRING, idFfblk, ffstr) != E_None)
		return 0;

	return forPatchCmdline(idVarname, idCmd, prefix);
}
