/* $Id$

	Internal command ICMDS

	ICMDS { [ '+' | '-' ] cmd }

 */


#include "../config.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/context.h"
#include "../include/nls.h"
#include "../err_fcts.h"
#include "../strings.h"

static char *icmdsChangeList(void)
{	struct CMD *cmd;
	char *q;
	char *p = 0;
	int buflen = 0;

	cmd = internalCommands;
	do if((cmd->flags ^ (cmd->flags >> 1)) & CMD_HIDDEN) {
		/* changed --> include into the list */
		if((p = efrealloc(p, buflen + strlen(cmd->name) + 3)) == 0)
			return 0;
		q = p + buflen;
		*q++ = ' ';
		*q++ = (cmd->flags & CMD_HIDDEN)? '-': '+';
		q = stpcpy(q, cmd->name);
		buflen = q - p;
	} while((++cmd)->name);

	return p;
}

int cmd_icmds(char *param)
{
	char **argv;
	int argc, opts, ec = E_None;

	if((argv = scanCmdline(param, 0, 0, &argc, &opts)) == 0)
		return 1;

	/* Because no option was passed into scanCmdline() no
		option can have been processed */
	assert(opts == 0);

	if(!argc) {
		char *p = icmdsChangeList();

		if(p) {
			puts(p);
			myfree(p);
		} else {
			ec = E_NoMem;
		}
	} else {
		char *p;
		for(argc = 0; (p = argv[argc]) != 0; ++argc) {
			int mode = 0;
			struct CMD *cmd;

			if(*p == '-') {
				mode = 1;
				++p;
			} else if(*p == '+')
				++p;
			StrFUpr(p);
			if((cmd = is_icmd(p)) != 0) {
				if(mode)	cmd->flags |= CMD_HIDDEN;
				else		cmd->flags &= ~CMD_HIDDEN;
/*			} else {	Ignore to allow ICMDS be intermixed with different
							variants of FreeCOM */
			}
		}

		if((p = icmdsChangeList()) != 0) {
			/* Update the settings within the context */
			char name[CTXT_ITEMNAME_LENGTH];

			ctxtMkItemName(name, CTXT_TAG_FLAG, CTXT_FLAG_ICMDS);
			ec = chgCtxt(CTXT_TAG_FLAG, name, p);
		} else
			ec = E_NoMem;
	}

	freep(argv);
	return ec;
}
