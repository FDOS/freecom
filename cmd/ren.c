/*
 * REN.C - rename command
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 * 15-Mar-2003 (Wolf Bergenheim <dog@users.sf.net>)
 * - added support for * and ?
 *
 */

#include "../config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

/*#define DEBUG*/

#include <dfn.h>
#include <supplio.h>

#include "../include/cmdline.h"
#include "../include/command.h"
#include "../err_fcts.h"

int cmd_rename(char *param)
{	
	char **argv;
	int argc, opts, ec = E_None;
	struct ffblk ff;


	if((argv = scanCmdline(param, 0, 0, &argc, &opts)) == 0)
		return 1;
	if(argc < 2) {
		error_req_param_missing();
		ec = E_Useage;
	}
	else if(argc > 2) {
		error_too_many_parameters(param);
		ec = E_Useage;
	}
	else {
		char *s_drv = NULL, *s_dir = NULL, *s_fil = NULL, *s_ext = NULL;
		char *d_drv = NULL, *d_dir = NULL, *d_fil = NULL, *d_ext = NULL;
//		char sn[MAXPATH+13], dn[MAXPATH+13];
		char *newname = 0, *sn = 0, *dn = 0;
		char *oldFname;

		if(FINDFIRST(argv[0], &ff
		 , FA_NORMAL|FA_DIREC|FA_ARCH|FA_SYSTEM|FA_RDONLY|FA_HIDDEN) != 0) {
			error_sfile_not_found(argv[0]);
			return 0;
		}

#define p oldFname
		if(!dfnsplit(argv[0], &s_drv, &s_dir, &s_fil, &s_ext)
		 || !dfnsplit(argv[1], &d_drv, &d_dir, &d_fil, &d_ext)
		 || (sn = dfnmerge(0, s_drv, s_dir, "", NULL)) == 0
		 || (p = realloc(sn, strlen(sn) + sizeof(ff.ff_name) + 1)) == 0) {
		/* build dir to use */
			error_out_of_memory();
			ec = E_NoMem;
			goto errRet;
		}
		sn = p;
		p = strchr(sn, '\0');
#undef p

		/* if drive or path in second arg, return with syntax error */
		if((d_drv != NULL) || (d_dir != NULL)) {
			error_syntax(argv[1]);
			ec = E_Useage;
			goto errRet;
		}

		if((dn = dfnmerge(0, s_drv, s_dir, d_fil, d_ext)) == 0) {
		/* build dir to use */
			error_out_of_memory();
			ec = E_NoMem;
			goto errRet;
		}

		do {
			if((newname = fillFnam(dn, ff.ff_name)) == 0) {
				error_out_of_memory();
				ec = E_NoMem;
				break;
			}
			strcpy(oldFname, ff.ff_name);
			dprintf(("rename(%s, %s)\n", sn, newname) );
			if(rename(sn, newname) != 0) {
				perror("rename");
				ec = E_Other;
				break;
			}
		} while(FINDNEXT(&ff) == 0);

	errRet:
		free(s_drv); free(s_dir); free(s_fil); free(s_ext);
		free(d_drv); free(d_dir); free(d_fil); free(d_ext);
		free(newname); free(sn); free(dn);
	}
	
	freep(argv);
	return ec;
}
