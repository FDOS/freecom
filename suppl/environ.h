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
 * $Locker$	$Name$	$State$
 *
 * header file for the environment handling functions
 *
 * All functions work over the process-local environment segment, thus,
 * allowing external (spawned) processes to alter the environment and to
 * use this environment as a template for the exec() system call.
 * The functions leave the additional strings at the end of the environment
 * segment intact.
 * The functions detect a moved or deleted environment segment, if this
 * acting is reflected by the PSP fields.
 * NO function upper-case the variable name. That's different to
 * common DOS implementations.
 * 
 * The environment segment must be a stand-alone segment allocated with
 * the DOS API function "Allocate Memory", DOS-48. The standard environment
 * is of that type.
 *
 * The low-level functions suppose, that no process accesses the
 * environment segment asynchronously (TSR, or device driver).
 *
 *	Implemented functions:
 	file		name		short description

 	environ.h	chgenv		change current working environment
 	environ.h	env_setGlbSeg	set current environment segment
 	environ.h	env_glbSeg	return current environment segment
 	env_chg.c	env_change	change variables of an environment
 	env_chk.c	env_check	check an environment for syntax errors
 	env_cpy.c	cpyenv		retrieve the contents of a variable of current env
 	env_cpy.c	env_get		copy the contents of a variable into a buffer
 	env_del.c	env_delete	delete (free) an environment
 	env_dvar.c	dupvar		duplicate the value of an env var into the heap
 	env_dvar.c	env_dupvar	duplicate the value of an env var into the heap
 	env_fora.c	env_forAll	spawn a function for all variables
 	env_fany.c	env_findAnyVar	find the first case-(in)sensitive variable
 	env_ftch.c	env_fetch	as env_dupvar(), but match DOS-ish & free() prv var
 	env_insv.c	env_insVarOffset	insert a new env entry before another one
 	env_mstr.c	env_master	return the environment segment of the master shell
 	env_mtch.c	env_matchVar	match a variable with an case-insensitive name
 	env_new.c	env_create	create & initialize a new & empty environment
 	env_nosp.c	env_noSpace	check if the segment can hold another variable
 	env_nust.c	env_nullStrings	set the string counter to zero
 	env_ovrw.c	env_ovrVarOffset	overwrite one env entry with a new one
 	env_put.c	putenv		insert a environment string, calls chgenv()
 	env_repl.c	env_replace	replace/duplicate an environment
 	env_scnt.c	env_strcounter	modifies the counter how many strings are in env
 	env_scpy.c	env_strcpy	retreive a string from the environment
 	env_sdup.c	env_strdup	duplicate a string from the environment into heap
 	env_sget.c	env_glbSeg_	return current environment segment (Micro-C only)
 	env_shel.c	env_shell	return env seg of the most current launched shell
 	env_sins.c	env_strinsert	insert a new string
 	env_size.c	env_resize	Shrink/Grow an environment by a delta value
 	env_size.c	env_newsize	set the size of an environment to a value
 	env_sput.c	env_strput	replace/delete/append a string
 	env_sset.c	env_setGlbSeg_	set current environment segment (Micro-C only)
 	env_strg.c	env_string	return the offset of a string of an environment
 	env_strs.c	env_strings	return the number of strings of an environment

	functions of mostly internal nature:

 	env_insv.c	env_appVar	append a variable to an environment segment
 	env_find.c	env_findVar	locate a variable in an environment segment
 	env_free.c	env_endOfVars	return offset of terminator of var portion
 	env_free.c	env_firstFree	return offset 1st unused byte of env seg
 	env_free.c	env_freeCount	return the number of unused bytes of env seg
 	env_len.c	env_varlen	return the number of bytes used by the variable
 	env_sub.c	env_subVarOffset	remove the variable starting at the offset

 * Steffen Kaiser
 * 01/29/96
 *
*/

#ifndef __ENVIRON_H
#define __ENVIRON_H

#include <portable.h>

#ifdef _MICROC_
#define ENV_WALKFUNC int *
#else
typedef int (*ENV_WALKFUNC)(void *, unsigned, unsigned);
#endif

#ifndef _MICROC_
/* In order to use these macros, <dos.h> must be #include'd */
#define env_glbSeg peekw(_psp, 0x2c)	/* return the current env seg addr */
#define env_setGlbSeg(a) pokew(_psp, 0x2c, (a))	/* set the current env seg addr */
#else
#define env_glbSeg env_glbSeg_()
#define env_setGlbSeg env_setGlbSeg_
#endif

extern unsigned env_resizeCtrl;
/* Standard settings:
	deny shrinking & moving, use UMBs, best fit */
#define ENV_SHRINKBELOW 1		/* resize below last used byte (will destroy
									environment, is necessary to delete env) */
#define ENV_ALLOWMOVE	2		/* re-allocate environment segment if grow
									fails */
#define ENV_USEUMB		4		/* when env moves, search in UMBs first */
#define ENV_FIRSTFIT	8		/* when env moves, use allocation stragegy First Fit */
#define ENV_LASTFIT		16		/* when env moves, use allocation stragegy Last Fit */
			/* If both ENV_FIRSTFIT & ENV_LASTFIT are set, behaviour is undefined */
			/* If none of the above are set, Best Fit is used */

/* Defines for the env_replace() function: */
#define ENV_DELETE		1		/* free old environment segment */
#define ENV_COPY		2		/* copy old environment into new
									segment, may destroy the environment, if
									new seg is smaller than old one. */
#define ENV_CLEAR		4		/* initialize new environment, so it does not
									contain neither variables nor strings */
#define ENV_CREATE		8		/* create a new environment */
#define ENV_FREECOUNT	16		/* minimum amount of unused bytes specified */
#define ENV_LIMITED		32		/* create smallest possible enviroment */


/*!!!!!!!!!
	All these functions replace segm by the current used environment if
	it is zero. The environment must be initialized with the environement
	variable area and the string area (in the simpliest case, three byes 0).
	!!!!!!!!!*/

#ifdef _MICROC_
unsigned env_glbSeg_(void);
/* Return the segment of the global environment segment */

void env_setGlbSeg_(const unsigned segm);
/* Set segm to be the global environment segment now. */
#endif

word env_create(const unsigned length);
/* Create a new environment of ¯length® bytes and initialize it as empty.

	How to allocate the block is controlled by the env_resizeCtrl
	variable (ENV_USEUMB, ENV_LASTFIT, ENV_FIRSTFIT).

	Return:
		0: allocation failed
		!0: segment address
*/

void env_delete(const word segm);
/*	Delete (free) environment. If it is the current used environment,
	it is set to 0.

*/

int cpyenv(const char var[], char dst[], const unsigned length);
int env_get(word segm, const char var[], char dst[], const unsigned length);
/* Copy length bytes from the beginning of variable var into dst.

	Return value:
		0: found & copied.
		1: found, but too long to copy completely. (length-1) bytes copied.
		2: not found
		-1: var == NULL || dst == NULL || length == 0
*/

char *dupvar(const char var[]);
char *env_dupvar(word segm, const char var[]);
/* Duplicate the contents of the environment variable var into the
	local heap. The memory is allocated via the malloc() function.
	dupvar() processes the environment segment 0 (zero).
	The caller must free() the string himself.

	If segm == 0, the current used environment is used.

	Return:
		NULL: var == NULL, variable not found, no memory
		else: pointer to the malloc'ed string
*/

char *env_fetch(word segm, const char var[]);
/* as env_dupvar(), but:
	+ var[] is matched with findAnyVar()
	+ the previous returned string is free()'ed automatically

	It provides the "normal" DOS behaviour, except that it can fetch
	non-uppercased variables.
*/

int putenv(const char *var);
/* Insert/Replace/Delete a variable in the environment.

	var must contain an '=' sign. The left portion becomes the name
	of the variable, the right its value. This function calls chgenv()
	to achieve its goal.

	If the value is an empty string, the variable will be deleted,
	otherwise the variable will be inserted or, if it is already
	inserted, replaced.

	Return value:
		0: Cannot insert variable.
		1: variable replaced or deleted.
		2: variable not found.
		3: variable newly inserted.
		-1: No '=' sign in var or var == NULL.
*/

#define chgenv(name,value) env_change(0,name,value)
int env_change(unsigned segm, const char var[], const char value[]);
/* Insert/Replace/Delete the variable name with the value.

	if value == NULL => delete variable
	if value == ""   => create a variable with no value
	if name  == NULL => Return -1 (error)
	if name  == ""   => create a variable without name

	Return value:
		0: Cannot insert variable.
		1: variable replaced or deleted.
		2: variable not found.
		3: variable newly inserted.
		4: no environment at all
		-1: var == NULL.
*/


int env_ovrVarOffset(word segm, word ofs, const char *varname
 , const char value[]);
/*
 *	Overwrite the variable at segm:ofs with the variable varname
 *	and its value.
 *
 *	segm:ofs must be start of a variable within the segment!
 *
 *	if value == NULL => delete variable
 *	if value == ""   => create a variable with no value
 *	if var  == NULL => keep the name of the variable
 *	if var  == ""   => create a variable without name
 *
 *	if segm == 0, the current used environment is processed.
 *
 *	Return value:
 *		0: variable overwritten / deleted
 *		1: Cannot insert variable.
 *		3: environment corrupt
 *		4: no environment at all
 */

int env_insVarOffset(word segm, word ofs, const char varname[]
	, const char value[]);
/*	
 *	Insert the variable _before_ the byte pointed to by segm:ofs
 *	Therefore the pair should point to the byte following the '\0'
 *	character of the preceeding variable.
 *
 *	if value == NULL => this call is ignored
 *	if var  == NULL => error
 *	if value == ""   => create a variable with no value
 *	if var  == ""   => create a variable without name
 *
 *	if segm == 0, the current used environment is processed.
 *
 *	Return value:
 *		0: variable inserted / ignored
 *		1: Cannot insert variable.
 *		3: var == NULL
 *		4: no environment at all
 */


#ifndef NO_ENV_REPLACE_PROTOTYPE
#ifdef _MICROC_
register word env_replace(word env /*, int mode, word segm/length*/);
#else
word env_replace(word env, int mode, ... /*word segm/length*/);
#endif
/* Replace/duplicate the environment env.
 *	Replace the (old) environment env. How is controlled by mode:
 *		ENV_DELETE: Delete segment env
 *		ENV_COPY: Copy the contents of the old env into the new env
 *			If the new env is smaller than the old one, the contents
 *			could be destroyed.
 *		ENV_CLEAR: Initialize the new env (no vars, no strings)
 *			ENV_COPY superceeds this flag.
 *		ENV_CREATE: Create a new environment, in this case, length is
 *			provided as argument #3.
 *		ENV_FREECOUNT: Only applicable with ENV_CREATE set, this means that
 *			the length specifies the amount of unused bytes in the new env.
 *			ENV_LIMITED superceeds this flag.
 *		ENV_LIMITED: Only applicable with ENV_CREATE set, limit the length
 *			of the created environment to the lowest possible one. This value
 *			is 4, if ENV_COPY is not set; env_firstFree(env), otherwise.
 *			The third argument need not to be specified.
 	Both env and segm must point to different,& existing environments.
 	A segment number of 0 (zero) is replaced by the current used environment.
 	If env is 0, the current used environment is updated with segm.

 	Return:	0 on failure
 			segm or the newly create environment, otherwise
 */
#endif

int env_matchVar(word segm, char var[]);
/* Try to match the variable var in the environment segm.

	Normally all environment functions within this package are case-sensitive.
	This is uncommon to DOS and may lead to improper functionality, but this
	allows to process uncommon variables, such as "windir" and "winbootdir".

	First, this function tries to locate var as it is given; second, this
	function performs a case-insensitve match. The constents of the var[]
	array is modified to the string of the first found match that it can
	be used as the input to all the other functions.

	If segm == NULL, the current used environment is processed.

	Return:
		1: var == NULL
		2: no match found, var[] unaltered
		3: no environment at all, var[] unaltered
		Bit mask:
		4: more than one case-sensitive match found, var[] unaltered
		8: more than one case-insensitive match found, var[] updated
		16: case-sensitve match found, var[] unaltered
		32: case-insensitve match found, var[] updated

	Note: If at least one of the bits 4 or 16 is set, the var[] is not
		modified.
*/

int env_findAnyVar(word segm, const char var[], word *ecase, word *icase);
/* Try to match the variable var in the environment segm.

	Normally all environment functions within this package are case-sensitive.
	This is uncommon to DOS and may lead to improper functionality, but this
	allows to process uncommon variables, such as "windir" and "winbootdir".

	*ecase receives the first case-sensitive (exact-case) match.
	*icase receive the first case-insentive match.

	Neither ecase nor icase may be NULL.

	If segm == NULL, the current used environment is processed.

	Return:
		0: sucess, *icase and *ecase altered
		1: var == NULL || icase == NULL || ecase == NULL
		2: no match found, var[] unaltered
		3: no environment at all, var[] unaltered
*/

int env_noSpace(word segm, const char name[], const char value[], const word ign);
/* Checks if the segment segm has enough free space to hold the
	variable "name=value". The amount of ign bytes are assumed to
	be additionally free.

	If segm == 0, the current used environment is used.
	name and value may be == NULL.

	Return:
		0: Enough space
		1: No space
		4: No environment at all
*/

word env_resize(word segm, int delta);
/* Resize the environment memory block by delta bytes (grow/shrink).
	Special features are controlled by the global variable
	env_resizeCtrl. See env_size.c for more information.

 	Return value:
 		0: no environment at all or resize failed
 		else:	if delta == 0: size of the environment block
 				if delta != 0: the (possibly new) segment address
 
*/

word env_newsize(word segm, unsigned length);
/* Resize the environment memory block to length bytes.
	This functions behaves like the env_resize() function.
*/

int env_forAll(unsigned env, ENV_WALKFUNC fct, void *arg);
/*
	fct is a pointer to a function of the type:
		int (*fct)(void *arg, unsigned env, unsigned ofs)
	where arg is arg of env_forAll() and env:ofs is the address of the
	current variable (start of its name).

	fct() is called once for each variable. This loops terminates, if
	there is no next variable or if fct() returns != 0.

	Return:	 0: all variables done (function never hit).
			-1: environment corrupted or no environment at all
		  else: function cancelled walk process.

*/

unsigned env_master(void);
/* Return the environment segment of the master shell (1st launched shell).
*/

unsigned env_shell(void);
/* Return the environment segment of the most current launched shell.
	This is the shell, which invoked the current application or the
	shell, which invoked the application that invoked the current one.
*/

/*********
	Environment String handling functions
	*********/

int env_nullStrings(word segm);
/* Set the counter how many strings are in the string table to zero.

	Return:
		0: OK
		1: no room to place the counter
		4: no environment at all
*/

int env_strings(const word segm);
/* Return the number of strings stored in the environment */

int env_strcounter(word segm, int diff);
/* Modifies the counter how many strings appended to the environment.

	Return:	The resulting counter
		or: 0, if there is no environment at all or no string is appended.
*/

word env_string(const word segm, int stringNr);
/* Returns the offset of the stringNr'th string of the environment segm.

	If stringNr == env_strings(), the offset of the first byte immediately
	following the last string is returned.

	Return:
		0: stringNr too high
		else: offset
*/

int env_strcpy(word segm, char buf[], const int len, const int stringNr);
/*	Copies the stringNr'th string (1st string is 0) of the environment
	segm into the buffer buf with the length len.

	Return:
		0:	OK
		1:	stringNr too high
		2:	buffer to small the hold the complete string, partially filled
		3:	no environment at all
*/

char *env_strdup(word segm, const int stringNr);
/*	Copies the stringNr'th string (1st string is 0) of the environment
	segm into the buffer newly malloc()'ed.

	If segm == 0, the current used environment is processed.

	Return:
		NULL: failure: stringNr too high, no environment, no memory
		else: pointer to the buffer
*/

int env_strput(word segm, const char buf[], const int stringNr);
/* Replaces the stringNr'th string (1st string is 0) of the environment
	segm by the ASCIZ string buf.

	If buf == NULL, the string is deleted, all following strings are
	shifted up one position.

	If stringNr == env_strings(segm), a new string is appended.

	Return:
		0:	OK
		1:	stringNr too high
		2:	environment too small
		3:	no environment at all
*/

int env_strinsert(word segm, const char buf[], const int stringNr);
/* Inserts a new string before the stringNr'th string of the environment
	segm.

	If stringNr == env_strings(segm), a new string is appended.

	Return:
		0:	OK
		1:	stringNr too high
		2:	environment too small
		3:	no environment at all
		4:	buf == NULL
*/



/*********
	Internally used functions
	*********/

/*!!!!!!!
	All of these functions requires that segm must not be zero.
	!!!!!!!!*/

unsigned env_findVar(const unsigned segm, const char var[]);
/* Return the offset of the variable var in the environment. -1 means
	not found. This function is case-sensitve. */

void env_subVarOffset(const unsigned segm, const unsigned offset);
/* Remove variable starting at offset */

int env_appVar(word segm, const char name[], const char value[]);
/* Append a new variable with the name and value
	It is not checked, whether such variable already exists.
	Return value and parameter see env_insVarOffset()
*/

int env_check(const unsigned segm);
/*	Check the environemnt, the return value:
 		0:	environment OK
 		1:	no environment at all (envseg is NULL)
 		2:	PSP corrupt (envseg points to an invalid memory block)
 		3:	variable's space corrupted
 		4:	no string table
 		5:	string table corrupted or no string table at all
*/

unsigned env_firstFree(const unsigned segm);
/* Return the offset of the first unused byte of the environment */

unsigned env_endOfVars(const unsigned segm);
/* Return the offset of the NUL terminator of the part of the environment
	segment for the variables.
*/

unsigned env_freeCount(const unsigned segm);
/* Return the number of bytes, which are unused in the environment segment.
*/

unsigned env_varlen(const unsigned segm, const unsigned offs);
/* Return the number of bytes the variable at this position allocates. */

#endif
