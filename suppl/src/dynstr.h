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
/* $Id$
   $Locker$	$Name$	$State$

	Dynamic string functions.

ob(ject): dynstr
su(bsystem): dynstr
ty(pe): 
sh(ort description): Basic data type to represent a dynamic string
lo(ng description): 
pr(erequistes): 
va(lue): a (char*) pointer, must be initialized prior use
re(lated to): 
se(condary subsystems): 
in(itialized by): user
wa(rning): 
bu(gs): 
co(mpilers): 

ob(ject): STR_SAVED_TOKENS
su(bsystem): dynstr
ty(pe): 
sh(ort description): data type for reentrant \tok{strtok()} function
lo(ng description): This data type is used to save to and restore from
	reentrant information of the function \tok{StrTokenize()}.
pr(erequistes): 
va(lue): Blackbox type used by \tok{StrTokSave()} and \tok{StrTokRestore()}
	only.
re(lated to): StrTokenize StrTokSave StrTokRestore
se(condary subsystems): 
in(itialized by): StrTokSave
wa(rning): 
bu(gs): 
co(mpilers): 

ob(ject): STR_QUOTES
su(bsystem): dynstr
ty(pe): 
sh(ort description): data type for quote-related functions
lo(ng description): This structure consists of members describing
	what quotes and character to be quoted the quote-related functions
	shall be aware of. The members enumerate the word delimiters,
	characters to be quoted, single character quotes, pairily used
	quotes and unpairily used quotes.
pr(erequistes): 
va(lue): 
re(lated to): StrQuote StrUnquote StrAppQStr
se(condary subsystems): 
in(itialized by): user
wa(rning): 
bu(gs): 
co(mpilers): 

ob(ject): StrTokStop
su(bsystem): dynstr
ty(pe): 
sh(ort description): End a \fct{StrTokenize()} inspection
lo(ng description): Instructres the function \fct{StrTokenize} to
	flush all internal values in order to return \fct{NULL} next time
	it is called and, therefore, definitely end a tokenizer loop \em{and}
	ensures that the string has been restored to its original value.
pr(erequistes): 
va(lue): none
re(lated to): StrTokenize
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#ifndef __DYNSTR_H
#define __DYNSTR_H

#ifndef _MICROC_
#include <string.h>
#endif
#include <portable.h>
#include "sstr.h"
#include "nls_c.h"

struct STR_SAVED_TOKENS_ {
	char *str_next_saved_tokens;
	char str_ch_saved_tokens;
};

#ifdef _MICROC_
#define STR_SAVED_TOKENS struct STR_SAVED_TOKENS_
#else
typedef struct STR_SAVED_TOKENS_ STR_SAVED_TOKENS;
#endif

/* Enumerates all quotes a string may be quoted with: */
/* If one member is equal to NULL or "", the particular value
	does not exist. */
struct STR_QUOTES_ {
	const char
		/* Characters that delimit two strings and must be quoted
			if they appear within a string. */
	 *str_delimiters
		/* Characters that must be quoted if they appear within a
			string. */
	,*str_toquote
		/* Characters that quote the immediately next character
			as the backslash '\' does in C. These quotes are
			still "active" within otherwise quoted strings. */
	,*str_singleQuotes
		/* Characters that enclose a quoted string. The same character
			appears before and after the string, just like the '"' in C. */
	,*str_pairyQuotes
		/* Characters that enclose a quoted string. The first character,
			the one on an even offset (first character has offset == 0),
			is the left quote, the second character is the right one.
			Compareble to parenthizes.  */
	,*str_unpairyQuotes
		/* Pairy and unpairy quotes (the left one is checked) that quote
			single quotes, too. */
	,*str_fullQuotes
	;
};
#ifdef _MICROC_
#define STR_QUOTES STR_QUOTES_
#else
typedef struct STR_QUOTES_ STR_QUOTES;
#endif

#ifdef _MICROC_
#define dynstr char *
#else
typedef char * dynstr;
#endif

int StriCmp(const char *s1, const char *s2);
char *StrDup(const char * const s);

#define StrChg(poi,size)	StrChg_(&(poi), (size))
char *StrChg_(char ** const poi, size_t len);

#define StrFree(str) StrRepl_(&(str), 0)
#define StrFree_(str) StrRepl_((str), 0)

#define StrRepl(dest,src) StrRepl_(&(dest), (src))
char *StrRepl_(char ** const dest, char * const src);

#ifdef _MICROC_
register char *StrConcat(int argcnt);
register char *EStrConcat(int argcnt);
#else
char *StrConcat(int argcnt, ...);
char *EStrConcat(int argcnt, ...);
#endif

char *StrTrim(char * const s);
char *StrChar(int ch);
char *StrStrip(char * const s, int ch);

#define StrCpy(dest,src) StrCpy_(&(dest), (src))
char *StrCpy_(char ** const dest, const char * const src);
/* Copy (duplicate) the string src[] into dest.
	If src == NULL, this function is equal to StrFree(dest).

	Return:
		NULL: malloc() failed || src == NULL
		else: *dest
*/

#define StrAppDelimStr(dst,src,q)		\
	StrAppDelimStr_(&(dst), (src), (q))
char *StrAppDelimStr_(char ** const dst, const char * const src
 , const STR_QUOTES * const quotes);

#define StrAppStr(dst,src)	StrCat_(&(dst), (src))
#define StrAppStr_	StrCat_
#define StrCat(dest,src) StrCat_(&(dest), (src))
char *StrCat_(char ** const dest, const char * const src);

#define StrAppQStr(dst,src,quote_struct)		\
	StrAppQStr_(&(dst), (src), (quote_struct))
char *StrAppQStr_(char ** const dest, const char * const src
	, const STR_QUOTES * const quotes);

#define StrAppChr(dest,chr) StrAppChr_(&(dest), (chr))
char *StrAppChr_(char ** const dest, char chr);

char *StrQuote(const char * const s, const STR_QUOTES * const quotes);

char *StrUnquote(const char * const s, const STR_QUOTES * const quotes);

int StrUnquoteTokenAppend(const char **src, const STR_QUOTES * const quotes
	, char ** const dst);

int StrUnquoteToken(const char **src, const STR_QUOTES * const quotes
	, char ** const dst);


char *StrTokenize(char *str, const char * const token);
#define StrTokStop() (void)StrTokenize(0, 0)
void StrTokSave(STR_SAVED_TOKENS *st);
void StrTokRestore(STR_SAVED_TOKENS *st);

char *StrLeft(const char * const s, size_t length);
char *StrRight(const char * const s, size_t length);
char *StrMiddle(const char * const s, size_t pos, size_t length);
char *StrTail(const char * const s, unsigned pos);

#ifdef _MICROC_
#define StrBeg strbeg
#else
int StrBeg(const char * const s1, const char * const s2);
#endif		/* !defined(_MICROC_) */

int StrCString(char *str);

char *StrWord(char *s);

char *StrChr(const char * const s, int ch);
size_t StrnLen(const char * const s, size_t len);
char *StrDupe(const char * const beg, const char * const end);

#define StrEnd Strend
//char *StrEnd(char *str);


extern void *Emalloc(size_t len);
extern void *Ecalloc(size_t len);
extern void *Erealloc(void *p, size_t size);
#define Esetsize(poi,size)	Esetsize_(&(void*)(poi), (size))
extern void *Esetsize_(void **poi, size_t size);
#define Eresize(poi,size)	Eresize_((void**)&(poi), (size))
extern void *Eresize_(void **poi, size_t size);
extern char *Estrdup(const char * const str);
#define Estrchg(var,str) Estrchg_(&(var),(str))
extern char *Estrchg_(char ** const pvar, const char * const str);
extern char *Estrdupe(const char * const start, const char * const end);

#endif
