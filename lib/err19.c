/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_env_var_not_found
void error_env_var_not_found(const char * const var)
{	displayError(TEXT_ERROR_ENV_VAR_NOT_FOUND, var);
}
