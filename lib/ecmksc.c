/* $id$

	Make a silent C context.

	Return:
		internal error code
*/

#include "../config.h"

#include <stdarg.h>

#include "ec.h"
#include "../include/context.h"

int ecMkSC(const char * const str, ...)
{
	va_list ap;

	va_start(ap, str);

	return ecMkvcmd(EC_CMD_SILENT, str, ap);
}
