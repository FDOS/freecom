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

	Declarations of the GETOPT subsystem.

	If macro GETOPT_LONG_NO_FLAG_MEMBER is defined before #including this
	header, the 'options' structure does not contain the 'flag'
	member.

*/

#ifndef __GETOPT_H
#define __GETOPT_H

#include <portable.h>

struct option {
	const char *name;
	int has_arg;
#ifndef GETOPT_LONG_NO_FLAG_MEMBER
	int *flag;
#endif
	int val;
};

struct getopt_save_t {
	int gs_optind, gs_opterr;
	const char *gs_optcur;
};

extern int	optind, 	/* index into argv[] array */
			optopt,		/* unmodified last-read character */
			opterr;		/* if non-zero, visually displays errors */
extern char *optarg;	/* address of argument of an argumented option */
extern const char *optcur;	/* address of next character to parse as
								option; if NULL, uses argv[optind] */

int getopt(int argc, const char * const * const argv, const char * const options);

int getopt_long(int argc, const char * const *  const argv
 , const char * const optstring
 , const struct option * const longopts, int * const longindex);
int getopt_long_only(int argc, const char * const * const argv
 , const char * const optstring
 , const struct option * const longopts, int * const longindex);
int getopt_long_noFlag(int argc, const char * const * const argv
 , const char * const optstring
 , const struct option * const longopts, int * const longindex);
int getopt_long_only_noFlag(int argc, const char * const * const argv
 , const char * const optstring
 , const struct option * const longopts, int * const longindex);

#ifdef GETOPT_LONG_NO_FLAG_MEMBER
#define getopt_long getopt_long_noFlag
#define getopt_long_only getopt_long_only_noFlag
#endif

#define getopt_save(s)	getopt_save_(aS(s))
void getopt_save_(struct getopt_save_t * const sbuf);
#define getopt_restore(s)	getopt_restore_(aS(s))
void getopt_restore_(struct getopt_save_t * const sbuf);
void getopt_init(void);

#endif
