/*	$Id$

	Display the result of a CRITER repeat check autofail

	$Log$
	Revision 1.1  2004/09/13 18:59:39  skaus
	add: CRITER: Repeat check autofail magic {Tom Ehlert/Eric Auer}

 */

#include "../config.h"

#include "../include/command.h"
#include "../include/misc.h"
#include "../err_fcts.h"
#include "../strings.h"

void critEndRepCheck(void)
{	unsigned critCount;

	if(0 != (critCount = critDisableRepeatCheck())) {
		displayString(TEXT_HIDDEN_CRITER, critCount);
	}
}
