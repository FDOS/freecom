/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_filename_too_long
void error_filename_too_long(const char * const fname)
{	displayError(TEXT_ERROR_FILENAME_TOO_LONG, fname);
}
