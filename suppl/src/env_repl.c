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
/*  $RCSfile$
   $Locker$	$Name$	$State$

ob(ject): env_replace
su(bsystem): env
ty(pe): 
sh(ort description): Replace an environment by another one
lo(ng description): Replaces or moves one environment to another one.
	The following modes control various features:
	\item\tok{ENV_DELETE}: Remove environment \para{env} on success
	\item\tok{ENV_COPY}: Copy the contents of the old environment into the
	new one. If the size of the new environment is less than the size of the
	old one, the contents could be destroyed.
	\item\tok{ENV_CLEAR}: Initialize the new environment to contain no
	variables and no strings. \tok{ENV_COPY} superceeds this flag.
	\item\tok{ENV_CREATE}: Create a new environment of the length specified as
	the third argument. If this flag is not
	set, the third parameter specifies an already created environment to
	be used.
	\item\tok{ENV_FREECOUNT}: Only applicable if \tok{ENV_CREATE} is set. The
	third argument specifies the amount of bytes that shall be unused in
	the new environment after copying the old one, if necessary. This flag
	is supperceeded by \tok{ENV_LIMITED}.
	\item\tok{ENV_LIMITED}: Only applicable if \tok{ENV_CREATE} is set. The
	length of the new environment is choosen as the lowest possible number.
	The third argument is ignored and need not to be specified at all.
	\nolist
	To duplicate an environment one can use:
	\example{|}
	|newEnv = env_replace(oldEnv? oldEnv: env_glbSeg
	|	, ENV_CREATE | ENV_FREECOUNT | ENV_COPY, 0)
	or
	\example{|}
	|newEnv = env_replace(oldEnv? oldEnv: env_glbSeg
	|	, ENV_CREATE | ENV_LIMITED | ENV_COPY)
	The first example creates an environment with the same size as the old
	one; the second one uses the smallest possible size to fit the old
	contents in.\newline
	If the \tok{?:} construct is not used to pass the old environment into
	the function \em{and} \tok{\para{oldEnv} == 0}, \tok{env_replace()}
	will also change the default segment to the newly created one. Unless the
	\tok{ENV_DELETE} flag is set, this could leave the old environment
	inaccessable within memory wasting resources.
pr(erequistes): 
re(lated to): env_create env_delete
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): \tok{0}: on failure: errno is assigned
	\list \tok{ENXIO}: if no source environment could be identified
		\item \tok{ENOMEM}: could not allocated enough memory
		\item \tok{E2BIG}: destination environment exceeds 64KB
		\item \tok{EINVAL}: specified destination environment segment is not present
		\item \tok{EEXIST}: source and destination environment are equal
	\endlist
	\item else: new environment
fi(le): env_repl.c

 */

#include "initsupl.loc"

#ifndef _MICROC_
#include <dos.h>
#include <string.h>
#endif
#include <portable.h>
#include "mcb.h"
#include "suppl.h"
#define NO_ENV_REPLACE_PROTOTYPE
#include "environ.loc"		/* don't include the prototype of env_replace() */
#include "fmemory.h"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

#ifdef _MICROC_
register unsigned env_replace(unsigned XOenv /*, int mode, unsigned segm/length*/)
#else
unsigned env_replace(unsigned Oenv, int mode, unsigned Xsegm)
#endif
{	unsigned env, segm;
	DBG_ENTER1
#define length segm

#ifdef _MICROC_
	unsigned Oenv, mode;

	env = nargs() * 2 + &XOenv;
	Oenv = *(unsigned *)env;
	mode = ((unsigned *)env)[1];
	segm = ((unsigned *)env)[2];
#else
	segm = Xsegm;				/* ensure that this parameter is valid */
#endif

	DBG_ENTER2("env_replace", "env")
	DBG_ARGUMENTS( ("env=%u, mode=0x%x, segm/length=%u", Oenv, mode, segm) )

	chkMem

	unless_Xsegm(env, Oenv) {
		eno_set(ENXIO);
		DBG_RETURN_U( 0)		/* no source environment */
	}

	DBG_ARGUMENTS( ("effective env=%u", env) )

	if(mode & ENV_CREATE) {		/* create a new environment */
		if(mode & ENV_LIMITED)
			length = env_firstFree(env);
		else if(mode & ENV_FREECOUNT) {
			if(addu(&length, env_firstFree(env))) {
				eno_set(E2BIG);
				DBG_RETURN_U( 0)
			}
		}
		if((segm = env_create(length)) == 0)
			DBG_RETURN_U( 0)
	}
	else {						/* the 3rd argument is a segment */
		unless_segm(segm) {
			eno_set(EINVAL);
			DBG_RETURN_U( 0)
		}
		if(segm == env) {
			eno_set(EEXIST);
			DBG_RETURN_U( 0)
		}
	}
	assert(segm);
	assert(env);

	chkMem
	
	if(mode & ENV_COPY)
#ifndef _MICROC_
#ifdef min
#undef min
#endif
#define min(a,b) (((a)<(b))?(a):(b))
#endif
		_fmemcpy(MK_FP(segm, 0), MK_FP(env, 0)
		 , min(mcb_length(segm), mcb_length(env)));
	else if(mode & ENV_CLEAR)
		ENV_CLEAR_SEGM(segm);		/* the first 3 bytes must be zero */

	chkMem
 	/* Update other global references by the new segment. If DELETE is
 		enabled, delete() won't find the old references anymore, thus,
 		keeps the new segments in those references */
 	if(!Oenv)
 		env_relocateSegment(env, segm);
 	
	chkMem
	
	if(mode & ENV_DELETE)
		env_delete(env);

	chkMem
	
	DBG_RETURN_U( segm)
}
