/*
 *  EXIT -- exits current instance of FreeCOM or leave all batch files
 *
 * set the exitflag to true
 */

#include "../config.h"

#include "../include/context.h"
#include "../include/command.h"

int cmd_exit(char *param)
{
	return qecAction(param, &doExit);
}
