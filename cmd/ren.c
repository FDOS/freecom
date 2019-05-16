/* $Id$
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
#include <string.h>

/*#define DEBUG*/

#include <dfn.h>
#include <suppl.h>
#include <supplio.h>

#include "../include/lfnfuncs.h"
#include "../include/cmdline.h"
#include "../include/command.h"
#include "../include/misc.h"
#include "../err_fcts.h"

void myfnsplit( const char *path,
                      char *buf,
                      char *drv,
                      char **dir,
                      char **name,
                      char **ext )
{
    const char* end;
    const char* p;
    const char* s;

    if( path[ 0 ] && path[ 1 ] == ':' ) {
        if( drv ) {
            *drv++ = *path++;
            *drv++ = *path++;
            *drv = '\0';
        }
    } else if( drv ) *drv = '\0';

    for( end = path; *end && *end != ':'; ) end++;

    for( p = end; p > path && *--p != '\\'/* && *p != '/'*/; )
        if( *p == '.' ) {
            end = p;
            break;
        }
    if ( ext ) {
        *ext = buf;
        for( s = end; ( *buf++ = *s++ ) != '\0'; );
    }

    for( p = end; p > path; )
        if( *--p == '\\'/* || *p == '/'*/) {
        /* 
         * '/' can't happen as a path seperator in FreeCOM because it's treated
         * as a switch character no matter where it's found
         */
            p++;
            break;
        }

    if( name ) {
        *name = buf;
        for( s = p; s < end; ) *buf++ = *s++;
        *buf++ = '\0';
    }

    if ( dir ) {
        *dir = buf;
        for( s = path; s < p; ) *buf++ = *s++;
        *buf = '\0';
    }
}

void myfnmerge( char *path, const char *drive, const char *dir,
                            const char *fname, const char *ext )
{
    if( *drive ) {
        strcpy( path, drive );
#if 0 /* Unused */
        if( !drive[ 1 ] )strcat( path, ":" );
#endif
    } else ( *path ) = 0;

    if( *dir ) {
        strcat( path, dir );
        if( *( dir + strlen( dir ) - 1 ) != '\\' )
            strcat( path, "\\" );
    }

    if( *fname ) {
        strcat( path, fname );
        if( ext && *ext ) {
#if 0 /* Unused */
            if( *ext != '.' ) strcat( path, "." );
#endif
            strcat( path, ext );
        }
    }
}

int cmd_rename(char *param)
{	
	char **argv;
	int argc, opts, ec = E_None;
	struct dos_ffblk ff;
	int appState;

	if((argv = scanCmdline(param, 0, 0, &argc, &opts)) == 0)
		return 1;

	appState = appendDisable();
	if(argc < 2) {
		error_req_param_missing();
		ec = E_Useage;
	}
	else if(argc > 2) {
		error_too_many_parameters(param);
		ec = E_Useage;
	} else if(dos_findfirst(argv[0], &ff
		 , FA_NORMAL|FA_DIREC|FA_ARCH|FA_SYSTEM|FA_RDONLY|FA_HIDDEN) != 0) {
			error_sfile_not_found(argv[0]);
		/* ec == E_None */
	} else {
#if 0   /*
         * There is some dynamic memory bug in relation to dfnmerge
         * Let's spare ourselves the whole mucking about in dynamic memory
         * thing, and we can use myfnmerge/myfnsplit to make everything work.
         * Unless of course someone wants to fix the bug in dfnmerge/split
         * So... the old code is still here if anyone wants to give it a shot
         * AND, if anyone tries to blame the _REGISTER usage in lfnfuncs.c as
         * the culprit, it already uses intr instead :-)
         */
		char *s_drv = NULL, *s_dir = NULL, *s_fil = NULL, *s_ext = NULL;
		char *d_drv = NULL, *d_dir = NULL, *d_fil = NULL, *d_ext = NULL;
		char *newname = 0, *sn = 0, *dn = 0;
		char *oldFname;

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
#else
        char s_drv[ MAXDRIVE ], d_drv[ MAXDRIVE ];
        char *s_dir, *d_dir, *d_fil, *d_ext;
        char s_buf[ MAXPATH ], d_buf[ MAXPATH ],
             *sn, dn[ MAXPATH ], newname[MAXPATH];

        myfnsplit( argv[ 0 ], s_buf, s_drv, &s_dir, NULL, NULL );
        myfnsplit( argv[ 1 ], d_buf, d_drv, &d_dir, &d_fil, &d_ext );
#endif

		/* if drive or path in second arg, return with syntax error */
		if((d_drv[ 0 ]) || (d_dir[ 0 ])) {
			error_syntax(argv[1]);
			ec = E_Useage;
			goto errRet;
		}

#if 0
		if((dn = dfnmerge(0, s_drv, s_dir, d_fil, d_ext)) == 0) {
		/* build dir to use */
			error_out_of_memory();
			ec = E_NoMem;
			goto errRet;
		}
#else
        myfnmerge( dn, s_drv, s_dir, d_fil, d_ext );
	/* d_buf no longer used after this */
	sn = d_buf;
#endif

		do {
            fillFnam(newname, dn, ff.ff_name);
#if 0
			if(newname[0] == 0) {
				error_out_of_memory();
				ec = E_NoMem;
				break;
			}
#endif
			myfnmerge( sn, s_drv, s_dir, ff.ff_name, NULL );
			dprintf(("rename(%s, %s)\n", sn, newname) );
			if(rename(sn, newname) != 0) {
				myperror("rename");
				ec = E_Other;
				break;
			}
			/*free(newname);*/
            newname[0] = 0;
		} while(dos_findnext(&ff) == 0);

	errRet:
        dos_findclose(&ff);
#if 0 /* free(newname) isn't even necessary because it is free'd above */
		free(s_drv); free(s_dir); free(s_fil); free(s_ext);
		free(d_drv); free(d_dir); free(d_fil); free(d_ext);
		free(newname); free(sn); free(dn);
#endif
	}
	
	appendRestore(appState);
	freep(argv);
	return ec;
}
