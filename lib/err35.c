/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include "../config.h"

#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

#undef dispCopy
void dispCopy(const char src[], const char dst[], int append)
{	displayError(TEXT_MSG_COPYING, src, append? "=>>": "=>", dst);
}
