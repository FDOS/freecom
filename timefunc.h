/*

   TimeFunc -- DOS Time Functions
   Copyright (C) 1998 Rob Lake

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Rob Lake can be contacted at
   email: rlake@cs.mun.ca

 */

#if !defined(__TIMEFUNC_H)
#define __TIMEFUNC_H

#ifdef _NO__DOS_TIME

/* The time structure, compatible with dostime_t */
struct dostime_t
{
  unsigned char hour;           /* Hours */
  unsigned char minute;         /* Minutes */
  unsigned char second;         /* Seconds */
  unsigned char hsecond;        /* Hundredths of seconds */
};

unsigned _dos_settime(struct dostime_t *);
void _dos_gettime(struct dostime_t *);

#endif

#endif
