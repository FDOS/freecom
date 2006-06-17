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

ob(ject): optcur
su(bsystem): getopt
ty(pe): L
sh(ort description): Next character processed by any getopt() function
lo: points to the next character to be visited in the next call to any getopt()
	function.\par
	If \tok{optcur == NULL}, the functions parses \arg{argv[optind]}
	as a new argument, thus, probes if it is an option at all.\par
va(lue): next character within current argument
re(lated to): getopt getopt_long getopt_long_only optind
se(condary subsystems): 
in(itialized by): static, but user-overridable
wa(rning):=
bu(gs): One option cannot exceed the length of \tok{INT_MAX - 1} bytes.
co(mpilers): 

ob(ject): optind
su(bsystem): getopt
ty(pe): L
sh(ort description): Index into \tok{argv[]} array
lo: Index into \tok{argv[]} array to identify the current argument to be
	processed by any of the getopt() functions.\par
	If getopt() reaches the end of an option string, \para{optind} is
	incremented, so that the next call to getopt() begins to
	interprete the next argument.
pr(erequistes): \tok{optind > 0} -or- \tok{argv[optind]} must result into
	a valid string pointer
va(lue): current argument results to \tok{argv[optind]}
re(lated to): getopt getopt_long getopt_long_only optchar
se(condary subsystems): 
in(itialized by): static, but user-overridable
wa(rning):=
bu(gs): 
co(mpilers): 

ob(ject): optarg
su(bsystem): getopt
ty(pe): 
sh(ort description): Argument of current option
pr(erequistes): When a \tok{getopt()} function returned successfully
	this variable contains:\item \tok{NULL}: if the option does neither
	has an argument nor has a boolean switch associated with.
	\item else: Points to the argument or the boolean switch
va(lue): NULL: if the current option has no argument
	\item else: pointer into a string of the \tok{argv[]} array
re(lated to): getopt getopt_long getopt_long_only
se(condary subsystems): 
in(itialized by): getopt getopt_long getopt_long_only; overwritten with
	each call to such a function
wa(rning):=
bu(gs): 
co(mpilers): 

ob(ject): optopt
su(bsystem): getopt
ty(pe): 
sh(ort description): Last read character from argument
pr(erequistes): This is the unmodified character that caused \tok{getopt()}
	to return. If \tok{getopt()} returns EOF or '?', \tok{optopt} contains
	the erroreous character or \tok{'\\0'} if no character was available.
va(lue): 0: no current character
	\item else: current character
re(lated to): getopt getopt_long getopt_long_only
se(condary subsystems): 
in(itialized by): getopt getopt_long getopt_long_only; overwritten with
	each call to such a function
wa(rning):=
bu(gs): 
co(mpilers): 


*/

#include "initsupl.loc"
int optind = 1, optopt = 0, opterr = 1;
char *optarg = 0;	/* use 0, to avoid #include'ing stdio.h */
const char *optcur = 0;
