/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_long_batchline
void error_long_batchline(const char * const fnam, long linenr)
{	displayError(TEXT_ERROR_LONG_LINE_BATCHFILE, linenr, fnam);
}
