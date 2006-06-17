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

ob: syslog_options
su(bsystem): syslog
ty(pe): L
sh(ort description): Options of current syslog
lo(ng description): Options passed to the last \tok{openlog()}
	call that really opened the logging activities.
pr(erequistes): 
va(lue): \endlist '|'-combination of the following flags:
	 \item\tok{LOG_CONS}:	NOT IMPLEMENTED -- NO EFFECT\newline
          write directly to system console  if  there  is  an
              error while sending to system logger
	\item \tok{LOG_NDELAY}: NOT IMPLEMENTED -- NO EFFECT \newline
          open the connection immediately (normally, the connection
          is opened when the first message is logged)
	\item \tok{LOG_PERROR}: print to stderr as well
	\item \tok{LOG_PID}: include PID with each message
	\item \tok{LOG_NOTIME}: unique to SUPPL \newline
		    do not prepend the date & time syslog() is called
re(lated to): openlog syslog
se(condary subsystems): 
bu(gs): 
co(mpilers): 

ob: syslog_facility
su(bsystem): syslog
ty(pe): L
sh(ort description): default syslog facility
lo(ng description): Facility passed to the last call to \tok{openlog()}
	that really opened the logging activities. This one is used for
	the \tok{syslog()} calls, if none is specified there.\par
	If none (\tok{== 0}) is specified in the calls to both \tok{openlog()}
	and \tok{syslog()}, \tok{LOG_USER} is used.\newline
	Only one facility can be used at one time.
pr(erequistes): 
va(lue): 
	\item \tok{LOG_AUTH}: security/authorization  messages  (DEPRECATED   Use
              LOG_AUTHPRIV instead) 
	\item \tok{LOG_AUTHPRIV}: security/authorization messages (private) 
	\item \tok{LOG_CRON}: clock daemon (cron and at) 
	\item \tok{LOG_DAEMON}: other system daemons 
	\item \tok{LOG_KERN}: kernel messages 
	\item \tok{LOG_LOCAL0}: reserved for local use
	\item \tok{LOG_LOCAL1}: reserved for local use
	\item \tok{LOG_LOCAL2}: reserved for local use
	\item \tok{LOG_LOCAL3}: reserved for local use
	\item \tok{LOG_LOCAL4}: reserved for local use
	\item \tok{LOG_LOCAL5}: reserved for local use
	\item \tok{LOG_LOCAL6}: reserved for local use
	\item \tok{LOG_LOCAL7}: reserved for local use 
	\item \tok{LOG_LPR}: line printer subsystem 
	\item \tok{LOG_MAIL}: mail subsystem 
	\item \tok{LOG_NEWS}: USENET news subsystem 
	\item \tok{LOG_SYSLOG}: messages generated internally by syslogd 
	\item \tok{LOG_USER}:				 default \newline
            generic user-level messages 
	\item \tok{LOG_UUCP}: UUCP subsystem 
re(lated to): openlog syslog
se(condary subsystems): 
bu(gs): 
co(mpilers): 

ob: syslog_logfile
su(bsystem): syslog
ty(pe): L
sh(ort description): File descriptor of log file
lo(ng description): Opened file descriptor of the log file
pr(erequistes): syslog_opencount > 0
va(lue): valid \tok{(FILE*)} descriptor of the log file
re(lated to): openlog
se(condary subsystems): 
bu(gs): 
co(mpilers): 

ob: syslog_fnam
su(bsystem): syslog
ty(pe): L
sh(ort description): Fully qualified filename of log file
lo(ng description): 
pr(erequistes): syslog_opencount > 0
va(lue): pointer to dynamically allocated & absolute file name of 
	the log file
re(lated to): openlog
se(condary subsystems): 
bu(gs): 
co(mpilers): 

ob: syslog_openmode
su(bsystem): syslog
ty(pe): L
sh(ort description): Mode how to open the log file
lo(ng description): Specifies which modes to be passed to the "open
	file" call, when the log file is really opened within \tok{openlog()}.\par
	On the very first call to \tok{openlog()} this mode specifies "overwrite",
	when the log activities has been disable by calling \tok{closelog()},
	on any subsequent calls to \tok{openlog()} the mode is "append to".
pr(erequistes): 
va(lue): A valid mode to open the log file (static array)
re(lated to): openlog
se(condary subsystems): 
in: statically, then adjusted in \tok{closelog()}
bu(gs): 
co(mpilers): 

ob: syslog_ident
su(bsystem): syslog
ty(pe): L
sh(ort description): Default \para{ident} for \tok{syslog}
lo(ng description): If no \para{ident} is passed into a call
	to \tok{syslog()}, this one is used.\par
	If no ident is specified (hence, \tok{syslog_ident == NULL},
	no ident is issued by \tok{syslog}.
pr(erequistes): syslog_opencount > 0
va(lue): NULL: no ident
	\item else: valid pointer to dynamically allocated ident string
re(lated to): openlog syslog
se(condary subsystems): 
bu(gs): 
co(mpilers): 

ob: syslog_opencount
su(bsystem): syslog
ty(pe): L
sh(ort description): Counts how often the syslog functions are enabled
lo(ng description): Each time \tok{openlog} is called, this variable
	is incremented; each time \tok{closelog} is called, it is
	decremented. When the value of \para{openlog} is zero, the
	logging functions are disabled and \tok{openlog} and \tok{closelog}
	either enable or disable the functions.
pr(erequistes): 
va(lue): 0: logging disabled
	\item else: logging enabled
re(lated to): openlog closelog
se(condary subsystems): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include "syslog.loc"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id$";
#endif

/* Internal variables */
int syslog_options;
int syslog_facility;
FILE *syslog_logfile = 0;
char *syslog_fnam = 0;
char syslog_openmode[] = "wt";		/* overwrite in first openlog() */
char *syslog_ident = 0;
int syslog_opencount = 0;			/* mark: logfile currently closed */
