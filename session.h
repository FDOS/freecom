/*
	Declaration for the session management.
		See SESSION.C for details
	In short: these functions and settings deal with the way
	FreeCom reacts when an external command is about to be executed.

 * 1999/07/07 ska
 *	started
 */

#ifndef __SESSION_H
#define __SESSION_H

	/* shall the messages remain in memory */
extern int persistentMSGs;

int saveSession(void);
int restoreSession(void);

#endif
