/* $Id$

	Internal command FD

	FD num_JFT num_SFT
 */


#include "../config.h"

#include <assert.h>
#include <io.h>
#include <stdio.h>

#include <suppl.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../err_fcts.h"

int cmd_fd(char *param)
{	unsigned jft, sft;

	if(2 == sscanf(param, "%u %u", &jft, &sft)) {
		unsigned jftLen;
		byte far *jftp;

		jftLen = peekw(_psp, 0x32);
		jftp = *(byte far**)MK_FP(_psp, 0x34);
		assert(jftp);
		if(jft < jftLen && sft <= 255) {
			close(jft);
			jftp[jft] = sft;
			return 0;
		}
	}
	error_syntax(param);
	return E_Syntax;
}
