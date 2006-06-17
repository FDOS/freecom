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

ob(ject): StrTokenize
su(bsystem): dynstr
ty(pe): 
sh(ort description): Nestable non-destructive string tokenizer
lo(ng description): In conjunction with the helper functions \fct{StrTokSave},
	\fct{StrTokRestore}, and \fct{StrTokStop} this function implements
	a non-destructive and nestable string tokenizer (\fct{strtok}). \par
	During the tokenize loop the string will be changed by overwriting
	a token by \tok{NUL} in order to return a proper C string. But when
	the function \fct{StrTokenize} returns \tok{NULL} to indicate that
	no further field could be found, it the value of the string is
	restored to its original value. To terminate a tokenize loop
	prior its normal end, the function \fct{StrTokStop} must be called. \par
	If \tok{str != NULL}, the tokenizer starts scanning the supplied string,
	otherwise the previously passed string is scanned for the next
	token.\newline
	If two tokens (== field delimiters) follow each other immediately,
	an empty field is returned. \newline
	The tokens argument may change each time this function is called.
pr(erequistes): token != NULL
va(lue): NULL: if there is no further field; the string has been restored
	to its original value then.
	\item else: pointer the the value of the next field, this field is
	part of the passed in string and should not be modified
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): When the string is modified during a tokenize loop, the
	results are unpredictable.
bu(gs): 
co(mpilers): 

ob(ject): StrTokSave
su(bsystem): dynstr
ty(pe): 
sh(ort description): Nestable non-destructive string tokenizer safer
lo(ng description): To make \tok{StrTokenize()} nestable any internally
	used variable can be saved locally to start a new \tok{StrTokenize()}
	loop, e.g.:
	\example{|}
	|function(...)
	|{	STR_SAVED_TOKENS st;
	|	char *p;
	|
	|	StrTokSave(&st);
	|	if((p = StrTokenize(string, delimiter_list1)) != NULL) do {
	|		// do something with the token in p[]
	|	} while((p = StrTokenize(NULL, delimiter_list2)) != NULL);
	|	StrTokRestore(&st);
	|}
	One can savely nest \tok{function()} itself or call some function within
	\em{do something} that use an identical \tok{StrTokSave()}/\tok{StrTokenize()}/\tok{StrTokRestore()} sequence. The tokenizing loop will work.
pr(erequistes): 
re(lated to): StrTokRestore, StrTokenize
se(condary subsystems): 
in(itialized by): 
wa(rning):
bu(gs): 
va: none
fi(le): dstrtoke.c

ob(ject): StrTokRestore
su(bsystem): dynstr
ty(pe): 
sh(ort description): Nestable non-destructive string tokenizer restorer
lo(ng description): Restores a previous state of the \tok{StrTokenize()}
	function previously saved by \tok{StrTokSave()}. See there for details.
pr(erequistes): 
se(condary subsystems): 
in(itialized by): 
wa(rning):
bu(gs): 
va: none
re(lated to): StrTokSave, StrTokenize
fi(le): dstrtoke.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#endif
#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

static char *nxtToken = 0;		/* pointer to previous scanned string */
static char ch;						/* previous token delimiter */

char *StrTokenize(char *str, const char * const token)
{	DBG_ENTER("StrTokenize", Suppl_dynstr)

	if(!str) {
		if((str = nxtToken) == 0		/* nothing to do */
		 || (*str++ = ch) == NUL)		/* end of string reached */
			DBG_RETURN_BS( nxtToken = 0)
	}

	if(!token || !*token)			/* assume all the string */
		nxtToken = 0;
	else {
		nxtToken = str - 1;
		while(!strchr(token, *++nxtToken));
		ch = *nxtToken;
		*nxtToken = NUL;
	}

	DBG_RETURN_S( str)
}

void StrTokSave(STR_SAVED_TOKENS *st)
{	DBG_ENTER("StrTokSave", Suppl_dynstr)

	if(st) {
		st->str_next_saved_tokens = nxtToken;
		st->str_ch_saved_tokens = ch;
	}
	DBG_EXIT
}

void StrTokRestore(STR_SAVED_TOKENS *st)
{	DBG_ENTER("StrTokRestore", Suppl_dynstr)

	if(st) {
		nxtToken = st->str_next_saved_tokens;
		ch = st->str_ch_saved_tokens;
	}
	chkHeap
	DBG_EXIT
}
