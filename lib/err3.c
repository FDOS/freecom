/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_too_many_parameters
void error_too_many_parameters(const char * const s)
{	displayError(TEXT_ERROR_TOO_MANY_PARAMETERS_STR, s);
}
