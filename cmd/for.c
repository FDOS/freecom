/*
 *  FOR.C - for command.
 *
 *	Synopsises:
 *		a) FOR { '%' } v  IN (...) DO ...
 *		b) FOR [{ '%' }] name [ '%' ] IN (...) DO ...
 *	v ::= a single alphabetic character
 *	name ::= a name; at least two characters that may be used in
 *		filenames as well; name != "IN"
 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/batch.h"
#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../err_fcts.h"
#include "../strings.h"

	/* If set --> toupper(varE[-1]) */
#define FLAG_HACKERY_FOR 1

typedef enum {
	OK, badVar, noIN, noParens, noDO, noCMD
} forErrors;

static forErrors checkFOR(char * param	/* in: command line */
	, char ** varS, char ** varE		/* out: start/end variable name */
	, char ** paramS, char ** paramE	/* out: start/end paramter list */
	, char ** cmd, int *flags)			/* out */
{
	assert(param);

	assert(varS);
	assert(varE);
	assert(paramS);
	assert(paramE);
	assert(cmd);
	assert(flags);

	*flags = 0;
	/* Check that first element is % then an alpha char followed by space */

	*varS = param;
	if(*param == '%') { 	/* Check for FOR %%%%v IN syntax */
		while(*++param == '%');
		if(isalpha(*param) && isargdelim(param[1])) {
			/* hackery FOR variable */
			*flags |= FLAG_HACKERY_FOR;
			*varE = ++param;
			goto varfound;
		}
	}

#if 1
	/* the standard release does not support normal variables as
		FOR variables */
	return badVar;
#else
	/* Use a normal variable; strip leading %'s */
	*varE = param = skipfnam(*varS = param);
	if(*param == '%')		/* ignore _one_ trailing percent sign */
		++param;
	if(!*param || !isargdelim(*param)
	 || *varE - *varS < 2
/*	 || (*varE - *varS == 2 && memicmp(*varS, "IN", 2) == 0) */ ) {
		return badVar;
	}
#endif

varfound:
	param = ltrimcl(param + 1);   /* skip whitespaces */

	/* Check next element is 'IN' */
	if(!matchtok(param, "in")) {
		return noIN;
	}

	/* Followed by a '(', find also matching ')' */
	if(*param != '(')
		return noParens;

	*paramS = param + 1;

	{ 	/* Search for right ) */
		int inQuote = 0;

		while(*++param)
			if(is_quote(*param)) {
				if(inQuote)
					inQuote = 0;
				else
					inQuote = *param;
			} else if(*param == ')' && !inQuote
				&& isargdelim(param[1])	/* COMMAND bug: sees right parens
											only if followed by whitespace */
					)
				goto rightParansFound;
		return noParens;
	}
	rightParansFound:
		*paramE = param;

	param = ltrimcl(param + 1);

	/* Check if DO follows */
	if(!matchtok(param, "do"))
		return noDO;

	/* Check that command tail is not empty */
	if(!*param)
		return noCMD;

	*cmd = param;
	return OK;
}

static int doFOR(char *varname, char *varE, char *param, char *paramE
	, char *cmd, int flags)
{	/* char *oldContents;
	char **argv;			/* pattern list * /
	int argc;
	int rv;	*/

	assert(varname);
	assert(varE);
	assert(param);
	assert(paramE);
	assert(cmd);

	*varE = 0;
	*paramE = 0;

/* OK all is correct, build the exec contexts */
#if 1
	assert(*varname == '%');
	{
		struct bcontext *new = newBatchContext();

		if(!new)
			return 1;

		if((bc->forproto = strdup(cmd)) == 0
		 || (bc->forvar = strdup(varname)) == 0) {
			error_out_of_memory();
			exit_batch();   /* remove the newly created batch context */
			return 1;
		}

		if(!setBatchParams(param)) { /* Split out list */
			exit_batch();
			return 1;
		}

		bc->shiftlevel = 1;     /* skip %0 <=> filename */
	}
	return 0;

#else
	rv = E_None;

	/* 1st: C/FORVAR|SET hidden context */
	if(*varname == '%') {	/* special FOR variable */
		switch(ctxtGetS(1, CTXT_TAG_IVAR, varname, &oldContents)) {
#ifdef DEBUG
		default:
			dprintf(("[FOR: Invalid return value from ctxtGetS]\n"));
			rv = E_None;
			break;
		case 1:		/* no such item */
#else
		default:
#endif
			rv = ecMkc("IVAR %@VERBATIM()", varname, (char*)0);
			break;
		case 0:		/* Got it */
			rv = ecMkc("IVAR %@VERBATIM()", varname, "="
			 , oldContents, (char*)0);
			myfree(oldContents);
			break;
		case 2:	/* Out of memory */
			error_out_of_memory();
			return 1;
		}
	} else {			/* normal variable */
		oldContents = getEnv(varname);
			/* getEnv() will also update the varname array with the current
				case of the particular characters */
		rv = ecMkc("SET /C %@VERBATIM()", varname, "=", oldContents, (char*)0);
	}

	if(rv == E_None) {
		if((argv = split(param, &argc)) == 0) {
			error_out_of_memory();
			return E_NoMem;
		}

		/* Make the F context */
		if(argc)
			rv = ecMkF(argv, argc, varname, cmd);
		/* else silently ignore an empty argument line */
		freep(argv);
	}

	return rv;
#endif
}


int cmd_for(char *param)
{
	/*
	 * First check syntax is correct : FOR %v IN ( <list> ) DO <command>
	 *   v must be alphabetic, <command> must not be empty.
	 *
	 * If all is correct build a new F exec context.
	 * If also preserves the old contents of the FOR variable
	 *
	 *	varname := name of FOR variable
	 *	param := parameters within '(...)'
	 *	cmd := command
	 */

	char *varname, *varE, *cmd, *paramE;
	int flags;

	switch(checkFOR(param, &varname, &varE, &param, &paramE, &cmd, &flags)) {
	case badVar:
		error_for_bad_var();
		return 1;
	case noIN:
		error_for_in();
		return 1;
	case noParens:
		error_for_parens();
		return 1;
	case noDO:
		error_for_do();
		return 1;
	case noCMD:
		error_for_no_command();
		return 1;
#ifdef DEBUG
	case OK:
		break;
	default:
		dprintf(("[FOR: Invalid return value from checkFOR()]\n"));
		return 1;
#endif
	}

	return doFOR(varname, varE, param, paramE, cmd, flags);
}

int cmd_for_hackery(const char *Xparam)
{	char *param;
	char *varname, *varE, *cmd, *paramE;
	int flags;

	if(!matchtok((char*)Xparam, "for")
	 || checkFOR((char*)Xparam, &varname, &varE, &param, &paramE, &cmd, &flags)
	 != OK)
		return 0;

		/* Ignore the return value as this is a FOR cmd now, but if it
			fails here, some other error caused it */
	doFOR(varname, varE, param, paramE, cmd, flags);

	return 1;
}
