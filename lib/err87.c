/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/message.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_bfile_read
void error_bfile_read(const char * const fnam, const unsigned long lcnt)
{	displayError(TEXT_ERROR_BFILE_READ, fnam, lcnt);
}
