/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_bfile_no_such_label
void error_bfile_no_such_label(const char * const fnam, const char * const label)
{	displayError(TEXT_ERROR_BFILE_LABEL, fnam, label);
}
