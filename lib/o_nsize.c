/* $id$

	Output functions

	Issue a \n if not that many characters are left on the line

*/

#include "../config.h"

#include "out.h"

void oNeedSpace(const int size)
{	if(size > oColumnsLeft())
		oNL();
}
