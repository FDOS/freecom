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

ob(ject): StrQuote
su(bsystem): dynstr
ty(pe): _H
sh(ort description): Quote a string
lo(ng description): 
 Quotes all whitespaces and quotes of a string.\par
	If \tok{s[]} need to be quoted, this is done in this order:
	\enum If there is a pairy quote, which is not used within \tok{s[]},
		the whole \tok{s[]} is quoted by it.
	\enum If there is an unpairy quote, that is not used
		within \tok{s[]}, the whole \tok{s[]} is quoted with this pair.
	\enum If there is a single quote, each character to be quoted is
		quoted with it.
	\endlist
	Note: It is assumed that there is no part of the string already quoted.
pr(erequistes): quotes != NULL
re(lated to): STR_QUOTES StrUnquote
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dstrappe.c
va: NULL: if \tok{s == NULL} (EINVAL) or on memory allocation error (ENOMEM)
	or no available quote (ENOENT)
	\item else: pointer to dynamically allocated memory

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#include <stdlib.h>
#endif
#include <portable.h>
#include "dynstr.h"
#include "eno.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

char *StrQuote(const char * const s, const STR_QUOTES * const quotes)
{
	char *result;
	char *metachars;
	const char *p;
	const char *q;
	int right, ch;
	static char buf_l[2] = " ";
	static char buf_r[2] = " ";

	DBG_ENTER("StrQuote", Suppl_dynstr)

	assert(quotes);

	DBG_ARGUMENTS( ("s=\"%s\"", s) )

	if(!s) {
		eno_set(EINVAL);
		DBG_RETURN_S(0)
	}

	DBG_ARGUMENTS( ("delimiter=\"%s\", toquote=\"%s\"", quotes->str_delimiters, quotes->str_toquote) )
	DBG_ARGUMENTS( ("Quotes: pairy=\"%s\", unpairy=\"%s\", single=\"%s\", full=\"%s\"", quotes->str_pairyQuotes, quotes->str_unpairyQuotes, quotes->str_singleQuotes, quotes->str_fullQuotes) )

	/* we concatenate all quote strings in order to pass this info
		to strpbrk() */
	chkHeap
	if((metachars = StrConcat(5
 	 , quotes->str_delimiters
 	 , quotes->str_toquote
 	 , quotes->str_singleQuotes
 	 , quotes->str_pairyQuotes
 	 , quotes->str_unpairyQuotes)) == 0)
 	 	/* memory allocation error */
		DBG_RETURN_S(0)

	chkHeap
	if(!*metachars || strpbrk(s, metachars) == 0) {
		/* no character to be quoted within the string */
		free(metachars);
		chkHeap
		DBG_RETURN_BS(eno_strdup(s))
	}

	/* find a suitable quote; first try the fullquotes, because then
		single quotes can be ignored completely */
	if((q = quotes->str_fullQuotes) != 0 && (ch = *q) != NUL) {
		do if(strchr(s, ch) == 0) {
			/* suiteable quote found */
			if(Strchr(quotes->str_pairyQuotes, ch)) {
				right = ch;
				goto quoteWholely;
			}
			/* else must be pairy quote */
			q = Strchr(quotes->str_unpairyQuotes, ch);
			assert(q);
			assert((q - quotes->str_unpairyQuotes) % 2 == 0);
			assert(q[1]);
			right = q[1];
			goto quoteWholely;
		} while((ch = *++q) != NUL);
	}

	/* pariy quotes are easier to understand */
	if((q = quotes->str_pairyQuotes) != 0 && (ch = *q) != NUL) {
		do if(strchr(s, ch) == 0) {
			/* suiteable quote found */
			right = ch;
			goto quoteWholely;
		} while((ch = *++q) != NUL);
	}

	/* unpairy quotes */
	if((q = quotes->str_unpairyQuotes) != 0 && (ch = *q) != NUL) {
		do if(strchr(s, ch) == 0) {
			/* suiteable quote found */
			q = Strchr(quotes->str_unpairyQuotes, ch);
			assert(q);
			assert((q - quotes->str_unpairyQuotes) % 2 == 0);
			assert(q[1]);
			right = q[1];
			goto quoteWholely;
		} while((ch = *++q) != NUL);
	}

	/* must quote each character individually */
	chkHeap
	if(!quotes->str_singleQuotes || !*quotes->str_singleQuotes) {
		/* no available quote at all */
		eno_set(ENOENT);
		free(metachars);
		chkHeap
		DBG_RETURN_S(0)
	}

	result = strdup("");
	p = s - 1;
	chkHeap
	while((ch = *++p) != NUL) {
		if((strchr(metachars, ch)		/* add the quote */
		  && !StrAppChr(result, *quotes->str_singleQuotes))
		 || !StrAppChr(result, ch)) {
		 	free(result);
		 	free(metachars);
			chkHeap
		 	DBG_RETURN_S(0)
		}
	}
	chkHeap
	DBG_RETURN_S(result)

quoteWholely:
	*buf_l = ch;
	*buf_r = right;

	chkHeap
	free(metachars);

	chkHeap
	DBG_RETURN_BS(StrConcat(3, buf_l, s, buf_r))
}
