/*
 *  FCTTABLE.C - table of internal functions.
 *
 */

#include <stdlib.h>

#include "../config.h"
#include "../include/command.h"


/* a list of all the internal functions, associating their command names */
/* to the functions to process them                                     */

struct IFCT internalFunctions[] =
{
	{"TEMPFILE",	fct_tempfile},
	{"IVAR",		fct_ivar},
	{"VERBATIM",	fct_verbatim},

	{0, 0}
};
