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

ob(ject): getopt_long
su(bsystem): getopt
ty(pe): 
sh(ort description): Return next option of the command line
lo(ng description): Scans the command line of the program, extracts
	and returns options from an already broken-up command line, this
	is an \tok{int argc} and \tok{char *argv[]} as passed from the
	system to the \tok{main()} function.
	Non-option arguments are not handled by \tok{getopt()}.\par
	Long options are prefixed by two instead of only one switch character
	and must match an option string wholely. Long options are matched
	case-insentively, too, and may have an argument attached, delimited
	by one argument sign from the option itself.\newline
	If an option string is prefixed by only one switch character, it is
	parsed as \tok{getopt()} would.\par
	If \tok{GETOPT_NO_FLAG_MEMBER} is defined prior #include'ing the
	\tok{<getopt.h>} header file, the structure \tok{options} does not
	include the \tok{flag} member, which is interpreted as \tok{NULL}
	always by \tok{getopt_long}.\par
	When a long option is parsed, the following rules apply:
	\item The whole option string upto the first argument or embedded option
	sign must case-insensitively match the string, specified by the
	\tok{name} member of the \para{lopts[]} arrays.
	\item Arguments must be separeted from the option by one argument
	sign.
	\endlist
	On match the following happens:
	\item If \tok{longindex != NULL}, the index into the \para{lopts[]}
		array is storred there.
	\item If \tok{has_arg} is equal to:
		\item \tok{0}: no argument is parsed, which usually gives an
			error the next time \tok{getopt_long()} is called.
		\item \tok{1}: an argument is required, which means an argument
			sign must be present immediately following the option.
		\item \tok{2}: if an argument sign follows the option, this
			string is returned as argument, thus, \tok{optarg} is non-zero
			on return; otherwise \tok{optarg} is zero.
		\item otherwise: undefined behaviour
		\endlist
	\item If \tok{flag == NULL}, \tok{val} is returned by \tok{getopt_long()};
		otherwise \tok{val} is storred into \tok{*flag} and \tok{getopt_long()}
		returns 0.
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

#define GETOPT_LONG
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
