/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_sfile_not_found
void error_sfile_not_found(const char * const f)
{	displayError(TEXT_ERROR_SFILE_NOT_FOUND, f);
}
