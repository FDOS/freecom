/*
 *  CANCEL -- leave all batch files
 *
 */

#include "../config.h"

#include "../include/context.h"
#include "../include/command.h"

int cmd_cancel(char *param)
{
	return qecAction(param, &doCancel);
}
