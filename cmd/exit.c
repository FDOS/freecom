/*
 *  EXIT -- exits current instance of FreeCOM or leave all batch files
 *
 * set the exitflag to true
 */

#include "../config.h"

#include "../include/context.h"

#pragma argsused
int cmd_exit(char *param)
{
	ctxtFlags.f_exit = 1;

	return 0;
}
