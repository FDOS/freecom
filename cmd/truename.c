/*
 * TRUENAME.C -- Truename Command (undocumented DOS?)
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
