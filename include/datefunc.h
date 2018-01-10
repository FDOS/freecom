/*

   DateFunc -- DOS Date Functions
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

#ifndef H__DATEFUNC_
#define H__DATEFUNC_

#if defined _NO__DOS_DATE || (defined __TURBOC__ && __TURBOC__ <= 0x296) || defined(__GNUC__) /* TC/TC++ 1.01 or GCC */

struct dosdate_t
{
  unsigned char day;            /* 1--31 */
  unsigned char month;          /* 1--12 */
  unsigned int year;            /* 1980--2099 */
  unsigned char dayofweek;      /* 0--6; 0 = Sunday */
};

unsigned _dos_setdate(struct dosdate_t *);
void _dos_getdate(struct dosdate_t *);

#endif /* _NO__DOS_DATE */
#endif /* H__DATEFUNC_ */
