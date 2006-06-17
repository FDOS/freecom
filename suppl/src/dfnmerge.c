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

ob(ject): dfnmerge
su(bsystem): dfn
ty(pe): H
sh(ort description): Put a filename together
he(ader files): 
lo(ng description): Constructs a filename from components, drive, path,
	filename and extension.\par
	The result string is:\newline
	<drive>:<path>\<name>.<extension>\newline
	The ':' is added, if <drive> is present.\newline
	The '\' is added, if <path> and (<name> or <ext>) are present.\newline
	The '.' is added, if <ext> is present.\par
	A passed-in value of \tok{NULL} indicates that the particular value is absent;
	the includes the empty string \tok{""}.\par
	If <drive> specifies an UNC path (\tok{dfnumerge()} only), ':' is
	substituted by one '\', which is not added, if neither <path>, nor
	<name> nor <ext> is present.\par
	If no result buffer is specified (\tok{\para{fnam} == NULL}), a
	buffer is dynamically allocated.\par
	Unless the macro \tok{SUPPORT_UNC_PATH} is defined prior \tok{#include}'ing
	the header file, UNC paths are not supported. Though, the only difference
	is that it is not checked if both filenames are UNC paths or both are not.
pr(erequistes): 
va(lue): NULL: out of memory\item \para{fnam}: if \tok{\para{fnam} != NULL}
	and no error\item else: pointer to dynamically allocated buffer
re(lated to): dfnumerge dfnsqueeze dfnexpand dfnsplit dfnpath
se(condary subsystems): 
in(itialized by): 
wa(rning): If \tok{path != NULL && *path == '\0'}, the path spec is
	considered to be absent. This is required to differ the result of
	\tok{dfnmerge(0, 0, 0, 0, 0)} from the root directory, when passed into
	\tok{dfnmerge()} as \para{path} again.
bu(gs): 
fi(le): dfnmerge.c

ob(ject): dfnumerge
su(bsystem): dfn
ty(pe): H
sh(ort description): Put a filename together
he(ader files): 
lo(ng description): As \tok{dfnmerge()}, but supports UNC paths.
pr(erequistes): 
va(lue): NULL: out of memory\item \para{fnam}: if \tok{\para{fnam} != NULL}
	and no error\item else: pointer to dynamically allocated buffer
re(lated to): dfnmerge dfnusqueeze dfnuexpand dfnusplit
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dfnmerge.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#endif
#include "eno.loc"
#include "dfn.loc"
#include "suppl.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

#define isDelim dfndelim

char *dfnmerge(char *fnam, const char * const dr, const char * const Xpath
 , const char * const nam, const char * const ext)
{	int len;
	const char *path;
	char *p;
	int delim;		/* type of last path component delimiter:
						0: none
						1: a "real" one ('/' or '\\') or none necessary
						2: a colon
					*/

#ifdef SUPPORT_UNC_PATH
	DBG_ENTER("dfnumerge", Suppl_dfn)
#else
	DBG_ENTER("dfnmerge", Suppl_dfn)
#endif
	DBG_ARGUMENTS( ("dr=\"%s\", pa=\"%s\", na=\"%s\", ex=\"%s\", out=%p", dr, Xpath, nam, ext, fnam) )

	path = Xpath;
	if((p = fnam) == 0) {	/* determine file nam length */
#ifdef SUPPORT_UNC_PATH
		len = dr? (*dr == '\\'? strlen(dr) + 1: 3): 1;
#else
		len = dr? 3: 1;		/* add the NUL terminator */
#endif
		if(path) {
			if(*path)
				len += strlen(path) + 1;
			else path = 0;	/* no path component specified */
		}
		if(nam) len += strlen(nam);
		if(ext) len += strlen(ext) + 1;
		if((fnam = p = eno_malloc(len)) == 0)
			DBG_RETURN_S( 0)
		*fnam = NUL;
	}

	if(dr) {
#ifdef SUPPORT_UNC_PATH
		if(*dr == '\\') {
			p = stpcpy(p, dr);
				/* Test if the drive spec already ends with a delimiter */
			delim = isDelim(p[-1]);
		}
		else {
#endif
			if(0 != (*p = *dr))
				++p;
			*p++ = ':';
				/* The colon is not a delimiter for root directories */
			delim = 2;
#ifdef SUPPORT_UNC_PATH
		}
#endif
		*p = NUL;
	}
	else delim = 1;		/* no drive --> no delim necessary */

	if(path) {
		switch(delim) {
		case 0:		/* missing delimiter --> need one unless path
						has one */
			if(!isDelim(*path)) *p++ = '\\';
			break;
		}
		if(!*path)		/* The root dir always requires a backslash */
			*p++ = '\\';
		/* Now, the delimiter is definitely there */
		p = stpcpy(p, path);
		if(0 == (delim = isDelim(p[-1]))) {
			if(p[-1] == ':')
				delim = 2;
		}
	}

	if(nam)	{
		if(!delim) {
			if(!isDelim(*nam))
				*p++ = '\\';
			delim = 1;
		}
		p = stpcpy(p, nam);
	}

	if(ext) {
		if(!delim) {
			*p++ = '\\';
			delim = 1;
		}
		*p++ = '.';
		strcpy(p, ext);
	}

	DBG_RETURN_S( fnam)
}
