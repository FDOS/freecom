/* $id$

	Helper function for Debugging and development purpose.

	Display "obsoleted" and terminate program.
*/

#include "../config.h"

#include <process.h>
#include <conio.h>

void obsoleted_(const char * const fnam, const int line)
{	cprintf("Function obsoleted in: %s line #%u\r\n"
	 "Press any key to terminate the program\r\n", fnam, line);

	getch();
	exit(124);
}
