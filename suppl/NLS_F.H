/* $Id$
   $Locker$	$Name$	$State$

	Declarations for NLS depended DOS filename functions.

   $Log$
   Revision 1.1  2000/07/09 22:19:22  skaus
   + Support for international strings without recompiling
   + Useage of TC++1

   Revision 1.1  2000/03/31 09:09:33  ska
   Initial revision

*/

#ifndef __NLS_F_H
#define __NLS_F_H

#include <portable.h>

int toFUpper(int ch);
int memcmpFI(const byte *buf1, const byte *buf2, unsigned len);
#ifdef _MICROC_
int _fmemcmpFI(unsigned const dseg, unsigned dofs
 , unsigned const sseg, unsigned sofs, unsigned length);
#else
int _fmemcmpFI(const byte far * dest, const byte * src, unsigned length);
#endif

#endif
