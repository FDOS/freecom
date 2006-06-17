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

ob(ject): suppl_log_enter
su(bsystem): debug
ty(pe): 
sy(nopsis): 
sh(ort description): Enter a new function and reconfigure logging status
he(ader files): 
lo(ng description): Enters a new function and reconfigures the logging
	status.\par
	The old status is preserved, if necessary, into the local data structure
	allocated within the stack frame of the function. The member \tok{fct_enabled} is newly configured from the passed in library name, function name and
	function class. The \tok{nestlevel} is incremented.\par
	The \tok{fct_enabled} follows the following rules:
	\enum If a library is specified (\tok{library_name != NULL}),
	this library must be explicitly enabled as function class.
	Otherwise logging is disabled for this function.
	\item If a filename is specified and the filename is explicitly
		mentioned, logging is either enabled or disabled.
	\item If both function_name and function_class are explicitly found,
	both must be enabled. Otherwise logging is disabled.
	\item If only one of them are explicitly found, its status is used.
	\item If neither of them are explicitly found, one default status
	must be enabled.
	\endlist
	\par
	By default no item is explicitly available and all default status'
	default to "disabled".
pr(erequistes): local_data != NULL && function_name != NULL && function_class != NULL
va(lue): none, but \item \tok{*data} is updated with data that must be
	preserved during one nest level
	\item the current nest level is incremented
	\item the function-depend enable status is re-evaluated
re(lated to): 
se(condary subsystems): 
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <string.h>
#endif
#include <portable.h>

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

void suppl_log_enter(suppl_log_sptr_t library_name
	, suppl_log_csptr_t file_name
	, suppl_log_linenr_t linenr
	, suppl_log_local_data_t *local_data
	, suppl_log_csptr_t function_name
	, suppl_log_csptr_t function_class)
{	int ch1, ch2;

	assert(local_data);
	assert(function_name);
	assert(function_class);

	/* preserve the current settings */
	local_data->suppl_l_prv_nestlevel = suppl_l_nestlevel;
	local_data->suppl_l_prv_fct_enabled = suppl_l_fct_enabled;
	local_data->suppl_l_fctname = function_name;

	/* Set the new values */
	++suppl_l_nestlevel;
	if(library_name) {
		if(suppl_log_find_list(&S(classes), library_name) != '+') {
			suppl_l_fct_enabled = 0;
			goto goon;				/* libraries are mandotary */
		}
	}

	if((ch1 = suppl_log_find_list(&S(files), file_name)) != 0) {
		suppl_l_fct_enabled = ch1 == '+';
		goto goon;
	}

	ch1 = suppl_log_find_list(&S(classes), function_class);
	ch2 = suppl_log_find_list(&S(functions), function_name);

	if(ch1)
		suppl_l_fct_enabled = ch1 == '+' && (!ch2 || ch2 == '+');
	else if(ch2)
		suppl_l_fct_enabled = ch2 == '+';
	else			/* Neither ch1 nor ch2 */
		suppl_l_fct_enabled = S(classes).suppl_l_name[0] == '+'
		 || S(functions).suppl_l_name[0] == '+';

goon:
	if(suppl_log_class_enabled("fct")) {
		/* Create the log entries when entering a new function */
		suppl_log_log_header();
		suppl_log_log_item(">%s() \"%s\"", function_name, function_class);
		if(library_name)
			suppl_log_log_item(" lib:%s", library_name);
		if(file_name)
			suppl_log_log_item(" in %s:%lu", file_name, linenr);
		suppl_log_log_trailer();
	}
}
