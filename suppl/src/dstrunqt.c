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

ob(ject): StrUnquoteTokenAppend
su(bsystem): dynstr
ty(pe): H
sy(nopsis): 
sh(ort description): Remove any quote from the next token of the string
he(ader files): 
lo(ng description): Removes any quote from the next token of the string and
	append the result to another string.\newline
	If quotes remain within the result, they were quoted within the
	string and therefore belong into the string.\newline
	Missing right quotes or single quotes immediately preceeding
	the '\0' byte are ignored silently.\newline
	Quotes may be embedded into tokens, e.g.:\example{|}
	|	"word"  qu"ote"d
	consists of four tokens:
	\enum one tab,
	\enum \tok{word},
	\enum two spaces, and finally
	\enum \tok{quoted}.
	\endlist
pr(erequistes): quotes != NULL && s != NULL && dst != NULL && (*dst == NULL || *dst == \text{dynamically allocated string})
va(lue):
	0: next token extracted; \tok{*s} has been assigned to the character
		following the last returned one and \tok{*dst} := pointer to
		dynamically allocated string where the new token was appened to
	\item else: out of memory failure (ENOMEM); \tok{*s} may be changed;
	\tok{*dst := NULL}, the original buffer has been deallocated
re(lated to): StrQuote StrUnquote StrUnquoteToken
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

ob(ject): StrUnquoteToken
su(bsystem): dynstr
ty(pe): H
sy(nopsis): 
sh(ort description): Remove any quote from the next token of the string
he(ader files): 
lo(ng description): Removes any quote from the next token of the string
	and assigns the result to dst.\newline
	The original value of \tok{*dst} is fully ignored, even no \tok{free()}
	is called.\newline
	If quotes remain within the result, they were quoted within the
	string and therefore belong into the string.\newline
	Missing right quotes or single quotes immediately preceeding
	the '\0' byte are ignored silently.\newline
	Quotes may be embedded into tokens, e.g.:\example{|}
	|	"word"  qu"ote"d
	consists of four tokens:
	\enum one tab,
	\enum \tok{word},
	\enum two spaces, and finally
	\enum \tok{quoted}.
	\endlist
pr(erequistes): quotes != NULL && s != NULL && dst != NULL
va(lue):
	0: next token extracted; \tok{*s} has been assigned to the character
		following the last returned one and \tok{*dst} := pointer to
		dynamically allocated string; if \tok{*dst == NULL} the
		end of the string was reached
	\item else: out of memory failure; \tok{*s} may be changed;
	\tok{*dst := NULL}
re(lated to): StrQuote StrUnquote StrUnquoteTokenAppend
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <portable.h>

#include "dynstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

int StrUnquoteTokenAppend(const char **s, const STR_QUOTES * const quotes
	, char ** const dst)
{
	const char *p;			/* temp pointer into s */
	size_t l;
	int ch;
	FLAG fullQuote;
	int endQuote;
	char *q;

	DBG_ENTER("StrUnquoteToken", Suppl_dynstr)

	assert(quotes);
	assert(s);
	assert(dst);
	assert(!quotes->str_unpairyQuotes
	 || strlen(quotes->str_unpairyQuotes) % 2 == 0);

	DBG_ARGUMENTS( ("str=\"%s\"", s) )

	*dst = 0;
	if((p = *s) == 0 || (ch = *p) == 0) {
		DBG_RETURN_I(0)
	}

	if((l = Strspn(p, quotes->str_delimiters)) != 0) {
		/* Unquoted delimiter
			--> The token is a sequence of delimiters */
		/** Warning: Possible incorrect assignment **/
		DBG_RETURN_BI( (*dst = StrDupe(p, *s += l)) == 0)
	}

	fullQuote = endQuote = 0;
	do {
		if(endQuote) {
			/* within quoted string */
			if(endQuote == ch) {
				/* end of quote reached */
				endQuote = 0;
				continue;
			}
			if(!fullQuote && StrChr(quotes->str_singleQuotes, ch)) {
				/* A not-full quote does not quote single quotes
					--> regardless what the next character is (except NUL)
					it is appended to the string */
				if((ch = *++p) == NUL)	/* single quote @ EOS --> ign */
					break;
			}
		} else if(StrChr(quotes->str_singleQuotes, ch)) {
			if((ch = *++p) == NUL)	/* single quote @ EOS --> ign */
				break;
		} else if(StrChr(quotes->str_pairyQuotes, ch)) {
			endQuote = ch;
			fullQuote = StrChr(quotes->str_fullQuotes, ch) != 0;
			continue;
		} else if((q = StrChr(quotes->str_unpairyQuotes, ch)) != 0
		 && (q - quotes->str_unpairyQuotes) % 2 == 0) {
		 	endQuote = q[1];
			fullQuote = StrChr(quotes->str_fullQuotes, ch) != 0;
			continue;
		} else if(StrChr(quotes->str_delimiters, ch)) {
			/* Unquoted delimiter --> Stop here */
			break;
		}
		if(!StrAppChr_(dst, ch)) { /* allocation error */
			StrFree_(dst);
			DBG_RETURN_I(1)
		}
	} while((ch = *++p) != NUL);

	*s = p;
	DBG_RETURN_I(0)
}


int StrUnquoteToken(const char **s, const STR_QUOTES * const quotes
	, char ** const dst)
{	DBG_ENTER("StrUnquoteToken", Suppl_dynstr)
	assert(dst);
	*dst = 0;
	chkHeap
	DBG_RETURN_BI(StrUnquoteTokenAppend(s, quotes, dst))
}
