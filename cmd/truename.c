/*
 * TRUENAME.C -- Truename Command (undocumented DOS?)
 *
 * 07/14/98 (Rob Lake)
 *  - started and tested fine
 *  - added stand alone debugging
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 28-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - changed _truename function so it does not return a pointer to a local
 *   variable.
 *
 * 09-Aug-1998 (Rob Lake <rlake@cs.mun.ca>)
 * - _truename now sets errno
 *
 */

#include "../config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <dfn.h>

#include "../include/command.h"
#include "../err_fcts.h"

int cmd_truename(char *param)
{
	char *p;

	if((p = dfntruename((param && *param)? param: ".")) == 0) {
		error_out_of_memory();
		return 0;
	}
	puts(p);
	free(p);

	return 1;
}
