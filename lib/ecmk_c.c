/* $id$

	Make a silent and hidden c context.

	Return:
		internal error code
*/

#include "../config.h"

#include <stdarg.h>

#include "ec.h"
#include "../include/context.h"

int ecMkc(const char * const str, ...)
{
	va_list ap;

	va_start(ap, str);

	return ecMkvcmd(EC_CMD_IGNORE_EXIT | EC_CMD_HIDDEN | EC_CMD_SILENT
	 , str, ap);
}
