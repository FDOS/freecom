/* $id$

	Output functions

	Display a string.

*/

#include "../config.h"

#include <dynstr.h>

#include "out.h"

void oStr(const char * const line)
{
	oMem(line, Strlen(line));
}
