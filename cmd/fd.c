/* $Id$

	Internal command FD

	FD num_JFT num_SFT
 */


#include "../config.h"

#include <assert.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>

#include <suppl.h>

#include "../include/command.h"
#include "../include/context.h"
#include "../err_fcts.h"

int cmd_fd(char *param)
{	unsigned fd, sft;

	if(2 == sscanf(param, "%u %u", &fd, &sft)) {
		byte far *jft;

		flushall();		/* messing around with file descriptors */

		jft = getJFTp();
		assert(jft);
		if(fd < getJFTlen() && sft <= 255) {
			dprintf(("[FD: Redirecting fd #%u (SFT %u --> %u)]\n"
			 , fd, jft[fd], sft));
#ifdef DEBUG
		if(0 !=
#endif
			close(fd)
#ifdef DEBUG
		) dprintf(("[FD: Failed to close fd #%u]\n", fd));
#else
		;
#endif
			jft[fd] = sft;
			/* in DOS redirections are text-mode */
			/* without this call, Borland C does not expand '\n' */
			setmode(fd, O_TEXT);
			return 0;
		}
	}
	error_syntax(param);
	return E_Syntax;
}
