/*
 *  WHERE.C - path functions.
 */

#include "../config.h"

#include <assert.h>
#include <stdio.h>

#include "../include/cmdline.h"
#include "../include/command.h"


int cmd_which(char *param)
{
	char **arg, *p;
	int argc, optc, i;

	if((arg = scanCmdline(param, 0, 0, &argc, &optc)) == 0)
		return E_Other;

	for(i = 0; i < argc; ++i) {
		assert(arg[i]);
		fputs(arg[i], stdout);
		if((p = find_which(arg[i])) != 0) {
			putchar('\t');
			puts(p);
		} else {
			putchar('\n');
		}
	}

	freep(arg);
	return E_None;
}
