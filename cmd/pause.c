/*
 *  PAUSE.C - pause command.
 *
 */

#include "../config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/command.h"
#include "../strings.h"

int cmd_pause(char *param)
{

	if(param && *param)
		fputs(param, stdout);
	else
		displayString(TEXT_MSG_PAUSE);

	cgetchar();
	putchar('\n');

	return 0;
}
