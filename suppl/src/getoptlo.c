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

ob(ject): getopt_long_only
su(bsystem): getopt
ty(pe): 
sh(ort description): Return next option of the command line
lo(ng description): Scans the command line of the program, extracts
	and returns options from an already broken-up command line, this
	is an \tok{int argc} and \tok{char *argv[]} as passed from the
	system to the \tok{main()} function.
	This function behaves like \tok{getopt_long()}, except that
	options with exactly one switch character are tried
	to match as long options, too; on failure, they are parsed
	as like \tok{getopt()}, but there must be only one option per
	option string. For instance:\example{|}
	|/longoption/l
	if \tok{'L'} is not included into the \para{lopts[]} array, it is
	parsed like a normal single character option, but if \tok{'LONGOPTION'}
	is not included into \para{lopts}, an error occurs, because it contains
	more than one characters. This applies to options with an argument as well.
pr(erequistes): \tok{\para{options} != NULL} and The characters in \para{options} must be upper-case.
	\item \tok{lopts != NULL} and the \tok{name} members must be upper-case.
values: \tok{EOF}: no further option character found, e.g. at end of command
	line or at an non-option argument 
	\item '?': Unknown option or \tok{hlpScreen()} returned. \tok{optopt}
	contains the character, which cased the error.
	\item 0: A long option was found and the \tok{flag != NULL}
	\item else: Upcased option character, and:\
		\item \tok{optopt == 0}, if a long option was found; otherwise it's
			the non-upcased option character 
		\item \tok{optarg}: non-zero if the option has an argument
re(lated to): getopt getopt_long_only optopt optarg optind optcur
se(condary subsystems): 
bu(gs): Non-7bit letters are not upcased.
	\item Whitespaces within option strings or arguments may or may not
	be supported by \tok{getopt()}; because the compiler's startup code
	prepares the values processed by \tok{getopt()}, this code must
	support whitespaces or better to say to support to \em{quote}
	whitespaces as they are typically used to delimit two arguments and/or
	options from each other.

*/
/*		Macros:
		GETOPT_LONG_ONLY: compiles getopt_long_only(), superceeds GETOPT_LONG
		GETOPT_LONG: compiles getopt_long()
		GETOPT_SILENT: no visual error reporting
		GETOPT_HELPSCREEN: if '?' is specified as option, hlpScreen() is
			invoked
		GETOPT_UPCASE: Upcase characters read from argument string as
			short option; optopt remains non-upcased
		GETOPT_UPCASE_LONG: Compare long options case-insensitively
		GETOPT_LONG_ALWAYS_FULL: don't match for partial long options
		GETOPT_EMBEDDED_OPTIONS: Ignore option signs embedded into options
		GETOPT_NEED_ARGSIGN: Short options need an argument sign
		GETOPT_NO_ARG_SPANNING: Don't use next argv[] entry as argument
		GETOPT_LONG_NO_FLAG_MEMBER: Don't include "flag" into struct option
		GETOPT_OPTSIGN_DASH: Support the dash as a second option sign, but
			not as embedded option sign
		GETOPT_ARGSIGN_EQUAL_SIGN: Support the equal sign as a second
			argument sign
 */

#define GETOPT_LONG_ONLY
#define GETOPT_UPCASE_LONG
#define GETOPT_UPCASE
#define GETOPT_ERROR_WRAPPER
#define GETOPT_HELPSCREEN
#define GETOPT_EMBEDDED_OPTIONS
#define GETOPT_NEED_ARGSIGN
#define GETOPT_NO_ARG_SPANNING
#define GETOPT_ARGSIGN_EQUAL_SIGN
#define GETOPT_ARGSIGN_COLON
#define GETOPT_OPTSIGN_SLASH
#include "getopt.src"
