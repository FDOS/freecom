/* $Id$
   $Locker$	$Name$	$State$

	NLS-depended dynamic string functions.

   $Log$
   Revision 1.1  2000/07/09 22:19:22  skaus
   + Support for international strings without recompiling
   + Useage of TC++1

   Revision 1.1  2000/03/31 09:09:33  ska
   Initial revision

*/

#ifndef __NLS_C_H
#define __NLS_C_H

#include <portable.h>

int toLower(int ch);
int toUpper(int ch);
int toFUpper(int ch);

#define memcmpI MemiCmp
int MemiCmp(const byte *buf1, const byte *buf2, unsigned len);

#define _fmemcmpI _fMemiCmp
#ifdef _MICROC_
int _fMemiCmp(unsigned const dseg, unsigned dofs
 , unsigned const sseg, unsigned sofs, unsigned length);
#else
int _fMemiCmp(const byte far * dest, const byte far * src, unsigned length);
#endif

int StriCmp(const char *s1, const char *s2);
char *StrUpr(char s[]);
char *MemUpr(char buf[], unsigned length);
void _fStrUpr(const word dseg, word dofs);
void _fMemUpr(const word dseg, word dofs, unsigned length);

int isodigit(int ch);
#define isodigit(ch)	((ch) >= '0' && (ch) <= '7')
int toxdigit(int ch);


#endif
