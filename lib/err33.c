/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef error_write_file_disc_full
void error_write_file_disc_full(const char * const fnam, const unsigned long size)
{	displayError(TEXT_ERROR_WRITE_FILE_DISC_FULL, fnam, size);
}
