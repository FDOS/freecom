/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_illformed_option
void error_illformed_option(const char * const s)
{	displayError(TEXT_ERROR_ILLFORMED_OPTION, s);
}
