/*	$id$
	$Locker$	$Name$	$State$

	Display the internal argument stack

	$Log$
	Revision 1.1.2.1  2001/07/16 20:28:38  skaus
	Update #9

 */

#include "../config.h"

#include "../include/command.h"
#include "../include/context.h"
#include "../strings.h"

#pragma argsused
int cmd_dispArgs(char *param)
{
	return ctxtView(CTXT_TAG_ARG, TEXT_ARG_CTXT_EMPTY);
}
