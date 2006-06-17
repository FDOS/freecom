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

ob(ject): Feof
su(bsystem): supplio
ty(pe): 
sh(ort description): Return the EOF status of a stream
lo(ng description): 
	Returns the EOF (end of file) status of the stream / \tok{(FILE*)} pointer.
	\par
	Note: Due to the fact that Micro-C does not have neither
	feof() nor ungetc(), this function returns true for all
	files opened in write mode, and uses fseek() to simulate an
	ungetc().
pr(erequistes): fp != NULL
va(lue): 0: not at EOF
	\item else: EOF reached
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): Micro-C only

*/

#include "initsupl.loc"

#ifdef _MICROC_
#include <portable.h>
#include <file.h>
#include "supplio.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int Feof(FILE *fp)
{	
	DBG_ENTER("Feof", Suppl_supplio)

	assert(fp);

	if((fp->FILE_options & F_WRITE) != 0 || getc(fp) == EOF)
		DBG_RETURN_I( 1)

	/* ungetc() the read character */
	fseek(fp, -1, -1, SEEK_CUR);
	DBG_RETURN_I( 0)
}
#endif
