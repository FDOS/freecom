/*
 *  QUIT -- exits current batch file (if any)
 *
 */

#include "../config.h"

#include "../include/context.h"
#include "../include/command.h"

int cmd_quit(char *param)
{
	return qecAction(param, &doQuit);
}
