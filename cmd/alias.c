/*
 *  ALIAS.C - alias administration module
 *
 */

#include "../config.h"

#include "../err_fcts.h"
#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../include/misc.h"
#include "../include/nls.h"
#include "../strings.h"

int cmd_alias(char *param)
{	char *value;

		/* Bail on every option */
	if(leadOptions(&param, 0, 0) != E_None)
		return 1;

	/* *param != whitespace */
	switch(breakVarAssign(ctxtAlias, param, &value)) {
	case 1:			/* no equal sign */
		error_syntax(0);
		return 1;
	case 0:			/* displayed */
		return 0;
#ifdef DEBUG
	case 2: break;
	default:
		dprintf(("[ALIAS: Invalid response from breakVarAssign()]\n"));
		return 1;
#endif
	}

	/* param[] == alias name -> check its validity */
	rtrimsp(param);		/* spaces are ignored at its end */
	if(!*param) {
		error_syntax(0);
		return 1;
	}
	if(!is_fnstr(param)) {
		error_no_alias_name(param);
		return 1;
	}
	StrFUpr(param);				/* Aliases are case-insensitive */
	value = trimsp(value);

	return chgCtxt(CTXT_TAG_ALIAS, param, *value? value: 0);
}
