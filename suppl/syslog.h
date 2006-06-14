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

	Implementation of the Unix-like syslog() function for DOS.
	The absent of an independed syslogd deamon caused some
	differences in the internal functionality of the function,
	meaning that some flags have no effect and that the call to
	openlog() is mandetory rather than optional.

	Also, the '%m' is not implemented right now and any useage of
	this token will result in undefined behaviour (sorry).

	See also the Linux MAN page: SYSLOG.MAN.

*/

#ifndef __SYSLOG_H
#define __SYSLOG_H

#include <stdarg.h>

/*****************************************
 ******** Constants for flags etc. *******
 *****************************************/
/*
 * openlog() options. Use a | combination of these:
 */
#define LOG_CONS 0			/* NOT IMPLEMENTED -- NO EFFECT */
          /*  write directly to system console  if  there  is  an
              error while sending to system logger */

#define LOG_NDELAY 0		/* NOT IMPLEMENTED -- NO EFFECT */
          /*  open the connection immediately (normally, the con-
              nection is opened when the first message is logged) */

#define LOG_PERROR 1
          /*  print to stderr as well */

#define LOG_PID 2
          /*  include PID with each message */

#define LOG_NOTIME 4		/* unique to SUPPL */
		  /*  do not prepend the date & time syslog() is called */

/*
 * openlog()/syslog() facilities. Use only one of these:
 * Note: the DOS implementation does not really use them, instead it
 * prepends them at the start of the line as a number.
 */
#define LOG_AUTH 1
           /* security/authorization  messages  (DEPRECATED   Use
              LOG_AUTHPRIV instead) */

#define LOG_AUTHPRIV 2
           /* security/authorization messages (private) */

#define LOG_CRON 3
           /* clock daemon (cron and at) */

#define LOG_DAEMON 4
           /* other system daemons */

#define LOG_KERN 5
           /* kernel messages */

#define LOG_LOCAL0 24
#define LOG_LOCAL1 25
#define LOG_LOCAL2 26
#define LOG_LOCAL3 27
#define LOG_LOCAL4 28
#define LOG_LOCAL5 29
#define LOG_LOCAL6 30
#define LOG_LOCAL7 31
           /* reserved for local use */

#define LOG_LPR 6
           /* line printer subsystem */

#define LOG_MAIL 7
           /* mail subsystem */

#define LOG_NEWS 8
           /* USENET news subsystem */

#define LOG_SYSLOG 9
           /* messages generated internally by syslogd */

#define LOG_USER 10				/* default */
           /* generic user-level messages */

#define LOG_UUCP 11
           /* UUCP subsystem */

/*
 * level -- importance of the log message. Can be |'ed to a facility
 * to construct a priority.
 * Use only one of these:
 */
#define LOG_EMERG 0x1000
           /* system is unusable */

#define LOG_ALERT 0x2000
           /* action must be taken immediately */

#define LOG_CRIT 0x3000
           /* critical conditions */

#define LOG_ERR 0x4000
           /* error conditions */

#define LOG_WARNING 0x5000
           /* warning conditions */

#define LOG_NOTICE 0x6000
           /* normal, but significant, condition */

#define LOG_INFO 0x7000
           /* informational message */

#define LOG_DEBUG 0x0000
           /* debug-level message */

/*****************************************
 ******** Functions **********************
 *****************************************/

void openlog(const char * const ident, int option, int  facility);
void syslog( int priority, char *format, ...);
void syslogo( int priority, char *format, ...);
void syslogv( int priority, char *format, va_list ap);
void closelog( void );

#endif
