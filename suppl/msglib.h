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

	Global message handling definitions.

*/

#ifndef __MSGLIB_H
#define __MSGLIB_H

#include <stdio.h>
#include <portable.h>

/*
 *	Some macros of message groups
 */
#define GLOBAL_GROUP 8
#define LOCAL_GROUP 0
#define msgIsGlobal(grp) ((grp) >= GLOBAL_GROUP)
#define msgIsLocal(grp)  ((grp) <  GLOBAL_GROUP)


#ifndef MSG_METHOD
	/* No method specified -> default to access method #1 */
#define MSG_METHOD	1
#endif /*#	!defined(MSG_METHOD) */

/*
 *	At this point exactly one of MSG_METHOD? is defined and valid
 */

#if MSG_METHOD == 1
#include <msglib/msgconf1.h>
#endif /*#	MSG_METHOD == 1 */

#if MSG_METHOD == 2
#include <msglib/msgconf2.h>
#endif /*#	MSG_METHOD == 2 */

	/* Source-In the initialization for all access methods */
#include <msglib/msgconf0.h>


/**REMOVE ON**/
#define Y_errMR "Error in message retreiver:\n"
#define Y_noMem "Out of memory\n"
#define MSG_errNrMR 125
/**REMOVE OFF**/


#endif /*#	!defined(__MSGLIB_H) */
