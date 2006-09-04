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
 * $Locker$	$Name$	$State$

 Declaration of the subsystem environ

ob(ject): 
su(bsystem): environ
ty(pe): 
sh(ort description): Type of the walkfunction passed to \fct{env_forAll}
lo(ng description): 
pr(erequistes): 
va(lue): 
re(lated to): 
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#ifndef __ENVIRON_H
#define __ENVIRON_H

#include <portable.h>

#ifdef _MICROC_
#define ENV_WALKFUNC int *
#else
typedef int (*ENV_WALKFUNC)(void *, word, word);
#endif

#ifndef _MICROC_
/* In order to use these macros, <dos.h> must be #include'd */
#define env_glbSeg peekw(_psp, 0x2c)	/* return the current env seg addr */
#define env_setGlbSeg(a) pokew(_psp, 0x2c, (a))	/* set the current env seg addr */
#else
#define env_glbSeg env_glbSeg_()
#define env_setGlbSeg env_setGlbSeg_
#endif

/* SUPPL may support two "default" segments:
	env_glbSeg - the global segment as defined by the PSP, and
	env_dfltSeg - an user-defined env segment, which is to replace the
		global segment fully (in regards of SUPPL)

	Environment functions, that excepts "0" as environent segment, expand
	use env_dfltSeg first, env_glbSeg then.

	The portable way is to use the function, though, for speed efficency
	they are implemented via a global variable.
*/
#define env_dfltSeg  (suppl_dfltEnvSegm)
#define env_setDfltSeg(value)	(suppl_dfltEnvSegm = (value))


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
word env_glbSeg_(void);
void env_setGlbSeg_(const word segm);
#endif

word env_create(const unsigned length);
void env_delete(word segm);
void env_clear(word segm);
int cpyenv(const char * const var, char * const dst, const unsigned length);
int env_get(word segm, const char * const var, char * const dst, const unsigned length);
char *dupvar(const char * const var);
char *env_dupvar(word segm, const char * const var);
char *env_fetch(word segm, const char * const var);
int putenv(const char *var);
#define chgenv(name,value) env_change(0,name,value)
int env_change(word segm, const char * const var, const char * const value);
int env_ovrVarOffset(word segm, word ofs, const char *varname
 , const char * const value);
int env_insVarOffset(word segm, word ofs, const char * const varname
	, const char * const value);

#ifndef NO_ENV_REPLACE_PROTOTYPE
#ifdef _MICROC_
register word env_replace(word env /*, int mode, word segm/length*/);
#else
word env_replace(word env, int mode, ... /*word segm/length*/);
#endif
#endif

int env_matchVar(word segm, char * const var);
int env_findAnyVar(word segm, const char * const var, word *ecase, word *icase);

int env_noSpace(word segm, const char * const name, const char * const value, const word ign);
word env_resize(word segm, int delta);
word env_setsize(word segm, unsigned length);
#define env_newsize(s,l)	env_setsize((s), (l))
int env_forAll(word env, ENV_WALKFUNC fct, void *arg);
word env_master(void);
word env_shell(void);

/*********
	Environment String handling functions
	*********/

int env_nullStrings(word segm);

/* int env_strings(const word segm); */
#define env_strings(segm)	env_strcounter((segm), 0)
int env_strcounter(word segm, int diff);
word env_string(word segm, int stringNr);
int env_strcpy(word segm, char * const buf, const int len, const int stringNr);
char *env_strdup(word segm, const int stringNr);
int env_strput(word segm, const char * const buf, const int stringNr);
int env_strinsert(word segm, const char * const buf, const int stringNr);

/*********
	DO NOT PASS ZERO INTO THESE FUNCTIONS!!!
	*********/


word env_findVar(const word segm, const char * const var);
void env_subVarOffset(word segm, word offset);
int env_appVar(word segm, const char * const name, const char * const value);

int env_check(const word segm);
/*	Check the environemnt, the return value:
 		0:	environment OK
 		1:	no environment at all (envseg is NULL)
 		2:	PSP corrupt (envseg points to an invalid memory block)
 		3:	variable's space corrupted
 		4:	no string table
 		5:	string table corrupted or no string table at all
*/

word env_firstFree(const word segm);
word env_endOfVars(const word segm);
word env_freeCount(const word segm);
unsigned env_varlen(const word segm, const word offs);
void env_relocateSegment(const word segm, const word tosegm);


/* Private variables made public for efficency purpose only.
	Any use of them is highly unportable! */
extern word suppl_dfltEnvSegm;

#endif
