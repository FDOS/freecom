/* $id$

	Perform a Quit/Exit/Cancel action:

	cmd [ errorlevel ]

*/

#include "../config.h"

#include "../include/command.h"
#include "../include/context.h"
#include "../include/misc.h"
#include "../err_fcts.h"

int qecAction(char *param, FLAG * const flag)
{	unsigned ec;

	if(param && *param) {
		if(is_num(param, &ec))
			F(errorlevel) = ec;
		else
			error_invalid_number(param);
	}

	*flag = TRUE;

	return 0;
}
