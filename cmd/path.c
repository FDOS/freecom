/*
 *  PATH.C - path command.
 *
 *  Comments:
 *
 * 17 Jul 1998 (John P Price)
 *   Seperated commands into individual files.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 12-Aug-1998 ska
 * - added: SUPPL environment handling
 *   currently "PATH=" will delete the environment variable PATH;
 *   if an empty one shall be created, replace chgEnv1() by chgEnv()
 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../strings.h"

#define PATHVAR    "PATH"

int cmd_path(char *param)
{

	if(!param || !*param) {
		displayString(TEXT_MSG_PATH, getEnv(PATHVAR));
		return 0;
	}

	if(*param == '=')
	/* skip whitespaces */
		param = ltrimcl(param + 1);

	rtrimcl(param);			/* remove trailing spaces */
	return chgEnvRemove(PATHVAR, param);
}
