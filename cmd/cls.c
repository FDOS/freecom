/*
 *  CLS.C - clear screen internal command
 *
 */

#include "../config.h"

#include <conio.h>

#include "../include/command.h"

#pragma argsused
int cmd_cls(char *param)
{
	textcolor(LIGHTGRAY);
	textbackground(BLACK);
	clrscr();
	return 0;
}
