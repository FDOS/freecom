/* $Id$
 *  TYPE.C - type internal command
 *
 *  Comments:
 *
 *  07/08/1998 (John P. Price)
 *    started.
 *
 * 07/12/98 (Rob Lake)
 *    - Changed error messages
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 10-Aug-1998 ska
 * - added ^Break checking
 *
 * 1999/01/24 ska
 * add: support for CP/M style device names (openf.h)
 */

#include "../config.h"

#include <assert.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../err_fcts.h"
#include "../include/openf.h"
#include "../strings.h"

int cmd_type(char *param)
{
	char buf[256];
	char **argv;
	int argc, opts, ec = E_None;
	int fd, len;

	if((argv = scanCmdline(param, 0, 0, &argc, &opts)) == 0)
		return 1;

	/* Because no option was passed into scanCmdline() no
		option can have been processed */
	assert(opts == 0);

	if(!argc) {
		error_req_param_missing();
		ec = E_Useage;
		goto errRet;
	}

	for(argc = 0; argv[argc]; ++argc) {
		if((fd = devopen(argv[argc], O_RDONLY)) < 0) {
			error_sfile_not_found(argv[argc]);
			ec = E_Other;
			break;
		}

		while((len = dos_read(fd, buf, sizeof(buf))) >= 0) {
			char *bufp, *p;
			if(cbreak) {
				dos_close(fd);
				ec = E_CBreak;
				goto errRet;
			}
			bufp = buf;
			for(p = buf; p < buf+len; p++) {
				if(*p == 26) break; /* CTRL-Z */
				if(*p == '\r' || *p == '\n') {
					if(p > bufp) dos_write(1, bufp, p - bufp);
					if(*p == '\n') dos_write(1, "\r\n", 2);
					bufp = p + 1;
				}
			}
			dos_write(1, bufp, p - bufp);
			if (len == 0 || *p == 26) break;
		}
		dos_close(fd);
		if(cbreak) {
			ec = E_CBreak;
			break;
		}
	}

errRet:
	freep(argv);

	return ec;
}
