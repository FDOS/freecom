/*	$id$
	$Locker$	$Name$	$State$

	Display the internal variables

	$Log$
	Revision 1.1.2.1  2001/07/16 20:28:38  skaus
	Update #9

 */

#include "../config.h"

#include "../include/command.h"
#include "../include/context.h"
#include "../strings.h"

#pragma argsused
int cmd_dispIVars(char *param)
{
	return ctxtView(CTXT_TAG_IVAR, TEXT_IVAR_CTXT_EMPTY);
}
