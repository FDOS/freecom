/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_no_alias_name
void error_no_alias_name(const char name[])
{	displayError(TEXT_ALIAS_INVALID_NAME, name);
}
