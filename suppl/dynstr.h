/*
    This file is part of SUPPL - the supplemental library for DOS
    Copyright (C) 1996-99 Steffen Kaiser

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
	Most of the functions take the first argument as the destination string
	and will be transformed into pointer by a macro.
	In this case, the pointer must be pre-initialized.

*/

#ifndef __DYNSTR_H
#define __DYNSTR_H

#ifndef _MICROC_
#include <string.h>
#endif
#include <portable.h>

/* Defines for StrSaveTokens() */
#define STR_RESTORE_TOKENS 1
#define STR_SAVE_TOKENS 0

struct STR_SAVED_TOKENS {
	char *str_next_saved_tokens;
	char str_ch_saved_tokens;
};


int toUpper(int ch);
/* Upcase character using the DOS NLS API.
	The upcase table is retreive only once during the program's run!
*/

int toFUpper(int ch);
/* Upcase character using the DOS NLS API according the table for
	filenames.
	The upcase table is retreive only once during the program's run!
*/

int MemiCmp(const byte *buf1, const byte *buf2, unsigned len);
/* Compare both memory area case-insensitvely.

	To make the compare case-insensitve the toUpper() function is used.

	If (buf1==NULL) || (buf2==NULL), the behaviour is undefined.
	If len == 0, MemiCmp() returns 0.

	Return:	
		 <0: buf1 is less then buf2
		==0: buf1 is equal to buf2
		 >0: buf1 is greater than buf2
*/

#ifdef _MICROC_
int _fMemiCmp(unsigned const dseg, unsigned dofs
 , unsigned const sseg, unsigned sofs, unsigned length);
#else
int _fMemiCmp(const byte far * dest, const byte far * src, unsigned length);
#endif
/* Compare both far memory area case-insensitvely.

	To make the compare case-insensitve the toUpper() function is used.

	If (buf1==NULL) || (buf2==NULL), the behaviour is undefined.
	If len == 0, _fMemiCmp() returns 0.

	Return:	
		 <0: buf1 is less then buf2
		==0: buf1 is equal to buf2
		 >0: buf1 is greater than buf2
*/

int StriCmp(const char *s1, const char *s2);
/* Compare two strings case-insensitively.

	To make the compare case-insensitve the toUpper() function is used.

	If (s1==NULL) || (s2==NULL), the behaviour is undefined.

	Return:	
		 <0: s1 is less then s2
		==0: s1 is equal to s2
		 >0: s1 is greater than s2
*/


char *StrDup(const char s[]);
/* Duplicate a string into the local heap.

	Return:
		NULL: s == NULL || malloc() failed
		else: pointer to the malloc'ed buffer
*/

#define StrChg(poi,size)	StrChg_(&(poi), (size))
char *StrChg_(char ** const poi, size_t len);
/* Reallocate the already initialized variable *poi.
	Return:
		NULL: on failure, *poi was free()'ed and is NULL
		else: pointer reallocated memory area == *poi
*/


#define StrFree(str) StrRepl_(&(str), NULL)
/* Free the string s[0] and place a NULL there

	Return: NULL
*/

#define StrRepl(dest,src) StrRepl_(&(dest), (src))
char *StrRepl_(char *dest[], char src[]);
/* Free dest[0] and place src there

	Return: src
*/

#ifdef _MICROC_
register char *StrConcat(int argcnt);
register char *EStrConcat(int argcnt);
#else
char *StrConcat(int argcnt, ...);
char *EStrConcat(int argcnt, ...);
#endif
/* Concat argcnt strings together and malloc() a buffer for the result.
	With Micro-C, argcnt is limited to the number of arguments that
	has been given in reality.

	EStrConcat() never returns NULL, but terminates the program with
	the "out of memory" error.

	Return:
		NULL: on failure (malloc() returned NULL)
		else: the malloc'ed buffer
*/

char *StrTrim(char s[]);
/* Reduces the malloc()'ed area of string s to the length (strlen(s) + 1).

	Return:
		NULL: if s == NULL || s cannot be shrinked (s is free()'ed!)
		else: a pointer to the new location (don't rely on this is s!)
*/

char *StrChar(int ch);
/* Create a new dynamic string containing the single character ch

	Return:
		NULL: malloc() returned NULL
		else: pointer to the malloc()'ed string
*/

char *StrStrip(char s[], int ch);
/* Remove any number of the character ch from the end of the string s.

	s[] may be NULL.
	ch may be 0.

	Return:	always s[].
*/

char *StrUpr(char s[]);
/* Upcase the string s[] using the toUpper() function.

	s may be NULL.

	Return:	always s
*/

char *MemUpr(char buf[], unsigned length);
/* Upcase the memory area buf[] using the toUpper() function.

	buf may be NULL.

	Return:	always buf
*/

void _fStrUpr(const word dseg, word dofs);
/* Upcases the string at dseg:dofs using toUpper().
*/

void _fMemUpr(const word dseg, word dofs, unsigned length);
/* Upcases the memory area at dseg:dofs using toUpper().
*/


#define StrCpy(dest,src) StrCpy_(&(dest), (src))
char *StrCpy_(char *dest[], const char src[]);
/* Copy (duplicate) the string src[] into dest.
	If src == NULL, this function is equal to StrFree(dest).

	Return:
		NULL: malloc() failed || src == NULL
		else: *dest
*/

#define StrCat(dest,src) StrCat_(&(dest), (src))
char *StrCat_(char *dest[], const char src[]);
/* Append the string src[] at dest.
	If src == NULL, this function performs no action.
	If *dest == NULL, this functions performs StrCpy_(dest, src)

	Return:
		NULL: malloc() failed; *dest is unchanged
		else: *dest
*/

#define StrAppend(dest,src,delim,quotes) \
	StrAppend_(&(dest), (src), (delim), (quotes))
char *StrAppend_(char *dest[], const char src[]
 , const char delim[], const char quotes[]);
/* Append the string src[] at dest delimited by *delim and quoted by a quote.

	If src == NULL, this function performs no action.

	If *dest == NULL, src[] is quoted if necessary.

	If *dest does not end with one character out of delim[] and src[] does not
	start with one character out of delim and **dest != '\0', *delim is
	placed between *dest and src[].

	delim[] consists of two fields:
		"*\0#"
	- All characters out of "*" are valid delimiting characters.
		Its first character is used as the delimiter.
	- All characters out of "#" are characters that are no delimiters, but
		must be quoted.

	quotes[] consists of three fields delimited by '\0':
		"*\0#\0[]"
	- All characters out of "*" delimit a string like '"' in C.
	- All characters out of "#" delimit a character like '\\' in C.
		These characters also quote every other quotation character.
	- All character pairs out of "[]" delimit a string like this:
		'[' mark the start of a quoted string;
		']' mark its end.
	If src[] need to be quoted, this is done in this order:
		1) If there is a quote out of "*" is not used within *dest,
			the whole src[] is quoted.
		2) If there is a quote pair out of "[]" is not used within *dest,
			the whole src[] is quoted with this pair.
		3) If there is a quote in "#", each character to be quoted is quoted
			with this quotation characterú
		4) src[] is not quoted.

	Return:
		NULL: malloc() failed; *dest is unchanged
		else: *dest
*/

#define StrAppChr(dest,chr) StrAppChr_(&(dest), (chr))
char *StrAppChr_(char *dest[], char chr);
/* Append a single character to the string *dest.

	chr may be NUL.
	*dest may be NULL.

	Return:
		NULL: malloc() failed
		else: pointer to the malloc()'ed buffer; original *dest free()'ed
*/

char *StrTokenize(char *str, const char token[]);
/* Tokenize str. str is unchanged, after StrTokenize() returned NULL.
	If str==NULL, the previous string is searched for the next token.
	If two tokens follow each other immediately, an empty string is returned.

	Return:
		NULL: no further token; the whole str is unchanged now
		else: pointer to token; this string's '\0' is the only modified part
				of the string.
*/

#define StrTokStop() (void)StrTokenize(NULL, NULL)
/* Make sure StrTokenize() will return NULL in the next call. The
	side effect is that the string is restored to its original
	value.
*/

void StrTokSave(struct STR_SAVED_TOKENS *st);
/* Save the current context of the StrTokenize() function into *st.

	If st==NULL, no action is performed.
*/

void StrTokRestore(struct STR_SAVED_TOKENS *st);
/* Restore the current context of the StrTokenize() function from *st.

	If st==NULL, no action is performed.
*/

char *StrLeft(const char s[], size_t length);
/* Return the left length characters of s.

	If strlen(s) < length, the string is duplicated.

	s may be NULL.

	Return:
		NULL: s==NULL, malloc() failed
		else: pointer to the malloc()'ed buffer
*/

char *StrRight(const char s[], size_t length);
/* Return the right length characters of s.

	If strlen(s) < length, the string is duplicated.

	s may be NULL.

	Return:
		NULL: s==NULL, malloc() failed
		else: pointer to the malloc()'ed buffer
*/

char *StrMiddle(const char s[], size_t pos, size_t length);
/* Return the length characters of a string beginning with the pos'th
	character. The first character has the position 0 (zero).

	If strlen(s) < pos, "" is returned.
	If strlen(s) < pos + length, StrTail(pos) is returned.

	s may be NULL.

	Return:
		NULL: s==NULL, malloc() failed
		else: pointer to the malloc()'ed string
*/

char *StrTail(const char s[], unsigned pos);
/* Return the substring beginning at position pos.

	If strlen(s) < pos, "" is returned.

	s may be NULL.

	Return:
		NULL: s==NULL, malloc() failed
		else: pointer to the malloc()'ed string
*/

#ifdef _MICROC_
#define StrBeg strbeg
#else
int StrBeg(const char s1[], const char s2[]);
/* Return if s1[] begins with s2[].

	Neither s1[] nor s2[] may be NULL.

	Returns:  >0 second block is greater
			 ==0 both are equal
			  <0 first block is greater
*/
#endif		/* !defined(_MICROC_) */

int StrCString(char *str);
/* Interprete C-style character strings. Control characters are left
	in the file except the string "\\\n" and a backslash at the end of
	the string, which are cut out of the string.

	Return:
		0: str == NULL
		else: number of bytes str[] now contains; the terminating NUL
			character is counted; str[] is modified
*/

int isodigit(int ch);
/* Test if ch is an octal digit.

	Return:
		0: ch is not '0'..'7'
		else: ch is no octal digit
*/

int toxdigit(int ch);
/* Transform a hexa-decimal digit into integer number.

	Return:
		0..15: hex-digit
		undefined: if ch is no hex-digit
*/

char *StrWord(char *s);
/* Reallocates the dynstr s and strips any leading and trailing blanks.

	Return:
		NULL: if s == NULL || realloc() fails
		else: new location (don't rely on == s)
*/

char *StrChr(const char s[], int ch);
/*	Searches for the character ch within s.

	Return:
		NULL: if s == NULL || ch == 0 || ch not in s
		else: pointer to the location of ch
*/

extern void *Emalloc(size_t len);
/*	Allocate a chunk of memory; on failure the program is
	terminated
*/

extern void *Ecalloc(size_t len);
/*	As getmem(); the allocated area is zero'ed
*/

extern void *Erealloc(void *p, size_t size);
/*	Resize an allocated memory area; on failure the program is
	terminated
*/

#define Esetsize(poi,size)	Esetsize_(&(void*)(poi), (size))
extern void *Esetsize_(void **poi, size_t size);
/* Sets the size of the dynamic memory block pointed to by *poi.
	The contents of the block may change.
	On failure the program is terminated with an "out of memory" error.

	Return:
		*poi
*/

#define Eresize(poi,size)	Eresize_(&(void*)(poi), (size))
extern void *Eresize_(void **poi, size_t size);
/* Reallocate the memory pointed to by *poi and stores the pointer of
	the new block into *poi.
	On failure the program is terminated with an "out of memory" error.

	Return:
		*poi
*/

extern char *Estrdup(const char * const str);
/*	Duplicate a string; on failure the program is terminated
*/

#define Estrchg(var,str) Estrchg_(&(var),(str))
extern char *Estrchg_(char ** const pvar, const char * const str);
/*	free(var/ *pvar); var/ *pvar = Estrdup(str)
*/

extern char *Estrdupe(const char * const start, const char * const end);
/* Duplicate the part of the string between start and end.
	if start == NULL, return NULL
	if end == NULL || end < start, return Estrdup(start)
*/

size_t StrnLen(const char * const s, size_t len);
/* Return the length of the string, but ensures to break after
	len bytes.

	if beg == NULL, return 0
*/

char *StrDupe(const char * const beg, const char * const end);
/* Duplicate the string beg, but not more than up to (not including)
	*end

	Return:
		NULL: beg == NULL || malloc() failed
		else: the duplicated (part of the) string
*/


#endif
