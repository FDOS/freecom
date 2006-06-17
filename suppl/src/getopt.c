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

ob(ject): getopt
su(bsystem): getopt
ty(pe): 
sh(ort description): Return next option of the command line
lo(ng description): Scans the command line of the program, extracts
	and returns options from an already broken-up command line, this
	is an \tok{int argc} and \tok{char *argv[]} as passed from the
	system to the \tok{main()} function.
	Non-option arguments are not handled by \tok{getopt()}.\par
	Option characters are single characters from the computers
	character set, except some special characters with a specific
	meaning as described below.\newline
	\item An option string is a combination of option characters, of which the
	last one may have an argument.
	\item The first character of an option string must be a forward slash.
	This character is called \em{switch character} or \em{option sign}.
	\item Option characters are matched case-insensitively.
	\item A switch character within an option string is ignored.
	\item The option character \tok{'?'} (question mark) yields function
		\tok{hlpScreen()}. This function may return.
	\item An argument must be seperated from the option by an argument
		sign, a colon \tok{':'} or an equal sign \tok{'='}.
	\item Options are not detected, if they are immediately attached to
		an argument as in: \tok{argument/option}.
	\item An argument of an option may be empty.
	\nolist
	Whitespaces are typically used to delimit arguments and options, however
	they are of no concern of \tok{getopt()}, because it gets passed an
	already broken-up command line; so the caller must ensure that whitespaces
	are handled correctly. \par
	\para{options} enumerates all option characters, they must be upcased.
	If a character is immediately followed by a colon (not a equal sign!),
	this option \em{requires} an argument; if it is followed by two colons,
	the option \em{may} have an optional argument. For instance, if
	\para{options} is: \tok{"1F:G::Hi"}:
	\item \tok{"/1f=argument"} succeeds two times, first option \tok{"1"}
		without an argument is returned, meaning \tok{optopt == '1'},
		\tok{optarg == NULL} and the return value is \tok{'1'}, too. The second
		time \tok{getopt} is called, \tok{'F'} is returned with
		\tok{optopt == 'f'} and \tok{optarg == "argument"}.
	\item \tok{"/1f"} retuns option \tok{'1'} as above,
		but fails on option \tok{'f'}.
	\endlist\par
	On failure, \tok{getopt()} returns the erroreous position within
	\para{optarg}, \para{optcur} is invalidated in order to force to advance
	to the next argument next time \tok{getopt()} is called, then
	\tok{'?'} is returned.\par
	If \tok{getopt()} finds a long option, see \tok{getopt_long()}, it
	issues an appropriate error message and then fails, but before
	the current options sign (the dash, if it is one, otherwise the
	slash) is assigned to \pata{optopt}.
pr(erequistes): \tok{\para{options} != NULL} && The characters in \para{options} must be upper-cased
values: \tok{EOF}: no further option character found, e.g. at end of command
	line or at an non-option argument 
	\item '?': Unknown option or \tok{hlpScreen()} returned. \tok{optopt}
	contains the character, which cased the error.
	\item else: Upcased option character, and:
		\item \tok{optopt}: non-upcased option character
		\item \tok{optarg}: non-zero if the option has an argument
		\endlist
re(lated to): getopt_long getopt_long_only optopt optarg optind optcur
se(condary subsystems): 
bu(gs): Non-7bit letters are not upcased.
	\item Whitespaces within option strings or arguments may or may not
	be supported by \tok{getopt()}; because the compiler's startup code
	prepares the values processed by \tok{getopt()}, this code must
	support whitespaces or better to say to support to \em{quote}
	whitespaces as they are typically used to delimit two arguments and/or
	options from each other.
ex(ample): \example{|}
	|int main(int argc, char **argv)
	|{	int opt;
	|	for(;;) {
	|		while((opt = getopt(argc, argv, "ABCD:E:F:")) != EOF)
	|			switch(opt) {
	|			case 'A':	// process non-argumented option 'A'
	|			case 'B':	// ...
	|			case 'E':	// argumented option 'E'
	|						// argument of option is pointed to by optarg
	|			case '?':	// error
	|			default:	// issue help screen
	|				return 127;
	|			}
	|		if(!argv[optind])	// no further argument on command line
	|			break;
	|		// process non-option argument argv[optind]
	|	}
	|	// reached here if all arguments of command line were scanned
	|	return 0;
	|}

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
