/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_alias_no_such
void error_alias_no_such(const char * const name)
{	displayError(TEXT_ERROR_ALIAS_NO_SUCH, name);
}
