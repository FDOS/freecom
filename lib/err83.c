/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/message.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_no_such_forced_internal_command
void error_no_such_forced_internal_command(const char * const name)
{	displayError(name? TEXT_ERROR_FAILED_FORCED_ICMD_STR: TEXT_ERROR_FAILED_FORCED_ICMD, name);
}
