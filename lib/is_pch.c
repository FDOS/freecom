/*	$id$
	$Locker$	$Name$	$State$

	int is_pathchar()

	is path character ::= normal filename characters
		| path delimiters | dot

	$Log$
	Revision 1.1.2.1  2001/07/08 17:23:43  skaus
	Update #7

 */

#include "../config.h"

#include "../include/misc.h"

int is_pathchar(const int c)
{
	return c == '.' || is_pathdelim(c) || is_fnchar(c);
}
