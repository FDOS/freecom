/*
    This file is part of SUPPL - the supplemental library for DOS
    Copyright (C) 1996-2000 Steffen Kaiser

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/* $RCSfile$
   $Locker$	$Name$	$State$

ob(ject): dfnsearch
su(bsystem): dfn
ty(pe): H
sh(ort description): Search for a file in a given set of paths
he(ader files): 
lo(ng description): Searches for a file in, if not already specified,
	a given set of paths and, if not already specified, with one of a
	set of given extensions.\par
	The individual components of \para{path} must be separated by a
	single semicolon ';' and must not contain superflous whitespaces. The
	behaviour of empty (zero-length components) is not defined. Each
	component is tested in the specified order.\newline
	The extensions must be delimited by a dot '.', they cannot contain
	a dot themselves. One leading dot will be ignored. Each extension
	will be tested in the specified order.\newline
	Both the filename and the extension may contain wildcards, which
	are passed unchanged to the DOS API and which are expanded with the
	first matching file.\newline
	If neither path nor extension is specified in the filename, first
	all extensions are probed within a path, then the next path component
	is used and all extensions are tested again.\par
	If there is no "." component in the \para{path}, the current working
	directory is tested at first.\newline
	If \tok{\para{path} == NULL}, the contents of the PATH environment
	variable is retreived and used.\newline
	If a path component is not fully-qualified, the result is not
	an abolsute one either.\newline
	If \tok{\para{ext} == NULL}, the list \tok{".COM.EXE.BAT"} will be
	used.\par
	Unless the macro \tok{SUPPORT_UNC_PATH} was defined prior \tok{#include}'ing
	the header file, this function does not support UNC paths.
pr(erequistes): 
va(lue): NULL: failure
	\list \tok{ENOMEM}: out of memory
		\item \tok{EINVAL}: Invalid argument, e.g. \tok{fnam == NULL}
	\endlist
	\item else: pointer to dynamically
	allocated complete filename
re(lated to): dfnusearch dfnsplit dfnmerge dfnpath
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): To search for an empty extension at the first place, one must
	specify two delimiter characters.
fi(le): dfnsearc.c

ob(ject): dfnusearch
su(bsystem): dfn
ty(pe): H
sh(ort description): Search for a file in a given set of paths
he(ader files): 
lo(ng description): As \tok{dfnsearch()}, but supports UNC paths.
pr(erequistes): 
va(lue): NULL: not found or out of memory or \tok{fnam == NULL}
	\item else: pointer to dynamically
	allocated complete filename
re(lated to): dfnsearch dfnusplit dfnumerge 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dfnsearc.c
*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <stdlib.h>
#include <io.h>
#ifndef _PAC_NOCLIB_
#include "dir.loc"
#endif
#else
#include <file.h>
#endif
#include "dfn.loc"
#include "dynstr.h"
#include "environ.h"
#include "suppl.h"
#include "eno.loc"

#include "suppldbg.h"

enum {
	OK, NOT_FOUND, ERROR
};

struct DFN_Search_Request {
	const char *basename;			/* search for <<path>>\basename.* */
	const char *extensions;
	char *foundMatch;
	int flags;
	char delim;
};

	/* Default search extensions */
#define DFLT_SEARCH_EXT "COM.EXE.BAT"
#define FILE_FIND_APPEND_PATTER ".*"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

static int matchFile(DFN_GLOB * const dir, const char * const ext)
{	const char *p;

	DBG_ENTER("dfnsearch::matchFile", Suppl_dfn)
	DBG_ARGUMENTS( ("ext=\"%s\"", ext) )

	assert(ext);
	assert(dir);
	p = dfnglobfilename(dir);
	DBG_ARGUMENTS( ("fnam=\"%s\"", p) )

	assert(p);
	DBG_RETURN_BI( dfnmatchext(p, ext) )
}

static int scanDir(struct DFN_Search_Request *r)
{	FLAG found = 0;
	DFN_GLOB *dir;
	int oldErrno = errno;

	DBG_ENTER("dfnsearch::scanDir", Suppl_dfn)
	DBG_ARGUMENTS( ("na=\"%s\", ext=\"%s\"", r->basename, r->extensions) )

	assert(r->basename);

	if(0 == (dir = dfnglobinit(r->basename, r->flags))) {
		DBG_RETURN_I(ERROR)
	}

	if(!r->delim) {		/* no extensions --> one match max */
		if(dfnglobread(dir)) {
			/* first match wins */
			found = 1;
			StrCpy(r->foundMatch, dfnglobfullfilename(dir));
		}
	} else while(dfnglobread(dir)) {		/* probe extensions */
		char *ext, *h;	/* start and end of extension */
		char delim = r->delim;

		assert(r->extensions);
		if(!*r->extensions) {	/* a previous search used up
									all extensions, hence, the best
									match is already found */
			break;
		}

		h = (char*)r->extensions - 1;
		while(0 != (h = strchr(ext = h + 1, delim))) {
			*h = 0;
			if(matchFile(dir, ext))
					/* no need to restore *h */
				goto foundIT;
			*h = delim;
		}
		/* The last item on the list, behind the last delimiter */
		if(!matchFile(dir, ext)) continue;	/* next file */
foundIT:
		found = 1;
		StrCpy(r->foundMatch, dfnglobfullfilename(dir));
		if(ext == r->extensions) {
			*ext = 0;
			break;		/* found best match ever */
		}
		ext[-1] = 0;
		/* Try next file with remaining extensions */
	}

	dfnglobfree(dir);
	if(errno == ENOENT)	/* This is no error in this case */
		eno_set(oldErrno);
	if(found) {
		if(r->foundMatch) {
			DBG_RETURN_I(OK)
		}
		/* Some error occured */
		DBG_RETURN_I(ERROR)
	}
	DBG_RETURN_I(NOT_FOUND);
}

char *dfnsearch(const char * const fnam
	, const char * const Xsearchpath
	, const char * const searchext)
{	char *na, *ex;	/* components of passed-in filename */
	const char *searchpath;
	int freeSearchpath = 0;
	int freeBasename = 0;
	int rc = ERROR;
	int oldErrno = errno;
	struct DFN_Search_Request r;

	DBG_ENTER("dfnsearch", Suppl_dfn)
	DBG_ARGUMENTS( ("fnam=\"%s\", path=\"%s\", extensions=\"%s\"", fnam, Xsearchpath, searchext) )

	if(!fnam || !*fnam) {
		eno_set(EINVAL);
		DBG_RETURN_S( 0)
	}

	r.flags = 0;
	r.delim = 0;
	r.basename = fnam;
	r.foundMatch = 0;
	r.extensions = 0;

/* What was specified? */
	chkHeap
	/*	na := filename
		ex := extension-like component */
	na = dfnfilename(fnam);		/* filename */
	ex = dfnfilenameext(na);
	if(*na == '.') {		/* filenames may start with a dot */
		/* Those files are hidden, usually */
		r.flags |= DFN_HIDDEN;
	}

	if(0 == *ex) {
		/*	The case *ex != 0 is handled like so:
			--> we can supply the full fnam as basename
				which has been setup above */

		/* Case 2: No extension --> we must supply the filename with
			appended '*' in order to initiate the file scan.

			If there is a path, we may supply it to dfnglob();
			if there is no path, one needs to be prepended on search,
			but we have only the filename in fnam.

			Ergo: we append '.*' to fnam and can pass it to scanDir()
			in both cases */

		if(0 == (r.basename
		 = StrConcat(2, r.basename, FILE_FIND_APPEND_PATTER)))
			/* memory allocation error */
			goto errRet;

		freeBasename = 1;

		/* Now propagate the extensions */
		if(searchext) {
			if(strchr(searchext, ';')) {
				/* extended format, use the string as is */
				r.extensions = searchext;
				r.delim = ';';
			} else {		/* dotted spec */
				if(*(r.extensions = searchext) == '.')
					++r.extensions;		/* first dot is optional */
				r.delim = '.';
			}
		} else {			/* use default extension */
			r.extensions = DFLT_SEARCH_EXT;
			r.delim = '.';
		}
		/* The list gets clobbered, therefore it is duplicated here */
		if(0 == (r.extensions = eno_strdup(r.extensions)))
			goto errRet;
	}

	/* Now: The request structure is filled, check if we need to
		cycle through %PATH% or searchpath */
	if(na != fnam) searchpath = 0;		/* drive or path --> no search */
	else if(0 == (searchpath = Xsearchpath)) {
		/* None specified --> try %PATH% */
		if((searchpath = dupvar("PATH")) != 0)
			 freeSearchpath = 1;
		 else {
			if(errno != ENOENT)		/* some other error */
				goto errRet;
			/* No PATH variable found */
		}
	}

	chkHeap
	if(searchpath) {			/* there is a search path (dr==pa==NULL) */
		/* check, if the current working directory must be searched */
		const char *p = searchpath - 1;
			/* scanCwd := first entry of %PATH% == "." ? */
		FLAG scanCwd = !(*searchpath == '.' &&
			(searchpath[1] == ';' || !searchpath[1]));
		while(scanCwd && (p = strstr(p + 1, ";.")) != 0)
			scanCwd = p[3] != ';' && p[3] != '\0';

		if(!scanCwd || NOT_FOUND == (rc = scanDir(aS(r)))) {
			STR_SAVED_TOKENS st;
			const char *base = r.basename;

			StrTokSave(aS(st));
			if((p = StrTokenize((char*)searchpath, ";")) != 0) do if(*p) {
				if(0 == (r.basename = dfnmerge(0, 0, p, base, 0))) {
					/* out of memory */
					rc = ERROR;
					StrTokStop();
					continue;
				}
				if(NOT_FOUND != (rc = scanDir(aS(r))))
					StrTokStop();
				free((/*drop const*/void*)r.basename);
			} while((p = StrTokenize(0, ";")) != 0);
			StrTokRestore(aS(st));
			r.basename = base;		/* In case it's dyn mem */
		}
		chkHeap
	} else {
		/* we use the supplied name itself, no cunning stuff */
		rc = scanDir(aS(r));
	}

errRet:
	if(freeSearchpath)
		free((/*drop const*/void*)searchpath);
	if(freeBasename)
		free((/*drop const*/void*)r.basename);
	free((/*drop const*/void*)r.extensions);			/* 0 by default */

	switch(rc) {
	case OK:
		eno_set(oldErrno);		/* may be clobbered by dfnglob() */
		DBG_RETURN_S(r.foundMatch)

	case NOT_FOUND:
		eno_set(ENOENT);		/* Not found ID */
		break;
	}

	DBG_RETURN_S(0)
}
