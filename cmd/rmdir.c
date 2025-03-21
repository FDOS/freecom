/* $Id$
 * RD / RMDIR - makes a call to directory_handler to do its work
 */

#include "../config.h"

#include <assert.h>
#include <dos.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "suppl.h"
#include "dfn.h"
#ifdef DBCS
# include "mbcs.h"
#endif

#include "../include/command.h"
#include "../include/misc.h"
#include "../include/lfnfuncs.h"
#include "../err_fcts.h"
#include "../strings.h"

#ifdef FEATURE_LONG_FILENAMES
#define abspath( x, y ) abspath( getshortfilename( x ), y )
#endif

/* recursively delete subdirs and files */
#ifdef DBCS
int rmdir_withfiles(const char * fullname, char * path, int maxlen)
#else
int rmdir_withfiles(char * path, int maxlen)
#endif
{
	struct dos_ffblk *f;
	int ret;
	int len = strlen(path);  /* Warning: we assume path is a buffer is large enough for longest file path */
	char *p = path+len;

	/* ensure ends with \ */
#ifdef DBCS
	if (*fullname && *(CharPrev(fullname, p)) != '\\')
#else
	if (p[-1] != '\\')
#endif
		*p++ = '\\';
	*p = '\0';
	
	dprintf(("rmdir(%s)\n", path));	

	/* cycle through removing directories first */
	stpcpy(p, "*.*");
	/* allocate our findfirst/next structure on heap to avoid exhausting stack */
	f = (struct dos_ffblk *)malloc(sizeof(struct dos_ffblk));
	if (f == NULL) {
		error_out_of_memory();
		return E_NoMem;
	}
	if (!dos_findfirst(path, f, FA_DIREC)) {
		ret = 0;
		do {
			/* skip . and .. directory entries */
			if ((strcmp(f->ff_name, ".")==0) ||
			    (strcmp(f->ff_name, "..")==0))
				continue;

			if (f->ff_attrib & FA_DIREC) {
				dprintf(("name=%s\n", f->ff_name));
				/* avoid overflowing our buffer */
				if((len + strlen(f->ff_name)) > maxlen) {
					error_filename_too_long(p);
					ret = E_Other;
					continue;
				}

				strcpy(p, f->ff_name);       /* Make the full path */
				/* recursively delete subdirectory */
#ifdef DBCS
				ret = rmdir_withfiles(fullname, path, maxlen);
#else
				ret = rmdir_withfiles(path, maxlen);
#endif
			}
		} while (!ret && (dos_findnext(f) == 0));
		dos_findclose(f);
		/* return early on error */
		if (ret) {
			free(f);
			return ret;
		}
	}

	/* remove any files in current directory */
	stpcpy(p, "*.*");
	if (!dos_findfirst(path, f, FA_NORMAL)) {
		ret = 0;
		do {
			/* avoid overflowing our buffer */
			if((len + strlen(f->ff_name)) > maxlen) {
				error_filename_too_long(p);
				ret = E_Other;
				continue;
			}

			strcpy(p, f->ff_name);       /* Make the full path */
			dprintf(("deleting [%s]\n", path));
			/* try to delete the file */
			if(unlink(path) != 0) {
				myperror(path);  /* notify the user */
				/* could exit here, but we just let rmdir fail */
			}
		} while (!ret && (dos_findnext(f) == 0));
		dos_findclose(f);
	}
	free(f);

	/* finally actually remove the directory */
	p[-1] = '\0';
	return rmdir(path);
}

int recursive_rmdir(const char * path, int recursiveMode, int quiet)
{
	if (recursiveMode) {
		struct dos_ffblk f;
		char *p;
		static char fullname[MAXPATH + sizeof(f.ff_name) + 2];
		int len;
		/* Get the pattern fully-qualified */
			/* Note: An absolute path always contains:
				A:\\
				--> It's always three bytes long at minimum
				and always contains a backslash */
		p = abspath(path, 1);
		if(!p)
			return E_Other;
		assert(strlen(p) >= 3);

		if((len = strlen(p)) >= MAXPATH) {
			error_filename_too_long(p);
			free(p);
			return E_Other;
		}
		strcpy(fullname, p);
		free(p);
		p = fullname + len;

		/* validate path exists and is a directory */
		if(!(dfnstat(fullname) & DFN_DIRECTORY)) {
			/* not a directory */
			return E_Other;
		}
		
		/* ensure ends with \ */
#ifdef DBCS
		if (len > 0 && *(CharPrev(fullname, p)) != '\\')
#else
		if (p[-1] != '\\')
#endif
			*p++ = '\\';
		*p = 0;
		
		/* prompt user if they are sure, regardless if files or not */
		if (!quiet) {
			int r;
			r = userprompt(PROMPT_DELETE_ALL, fullname);  /* Are you sure? TODO fix me */
				
			if (r != 1) {
				return E_Other;
			}
		}
#ifdef DBCS
		return rmdir_withfiles(fullname, fullname, sizeof(fullname));
#else
		return rmdir_withfiles(fullname, sizeof(fullname));
#endif
	} else {
		return rmdir(path);
	}
}

int cmd_rmdir(char *param)
{
	return mk_rd_dir(param, recursive_rmdir, "RMDIR");
}
