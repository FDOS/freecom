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

/*
 * 07/29/1998 (Rob Lake)
 *      started
 *
 * Works Under:
 *      Borland C++ v.3.0
 *      Pacific C (pacc v.7.51)
 *
 */

#ifdef _NO__DOS_DATE

#include <assert.h>
#include <dos.h>
#include <stdio.h>
#include "datefunc.h"

unsigned _dos_setdate(struct dosdate_t *d)
{
  union REGPACK r;
  assert(d);
  r.x.ax = 0x2B00;
  r.x.cx = d->year;
  r.x.dx = d->month << 8;
  r.x.dx += d->day & 0xFF;

  intr(0x21, &r);

  if (r.x.ax & 0x00FF)
    return 1;
  return 0;
}

void _dos_getdate(struct dosdate_t *d)
{
  union REGPACK r;

  assert(d);

  r.x.ax = 0x2A00;

  intr(0x21, &r);

  d->year = r.x.cx;
  d->month = r.x.dx >> 8;
  d->day = r.x.dx & 0xFF;
  d->dayofweek = r.x.ax & 0xFF;
}

#ifdef DEBUG_STANDALONE
int main(void)
{
  struct dosdate_t d,
    orig_d,
    new_d;

  _dos_getdate(&orig_d);
  printf("The current year is: %d\n", orig_d.year);
  printf("The current day is: %d\n", orig_d.day);
  printf("The current month is: %d\n", orig_d.month);
  printf("Day of week: %d\n", orig_d.dayofweek);

  new_d = orig_d;
  new_d.day++;
  if (_dos_setdate(&new_d) != 0)
    return 1;

  _dos_getdate(&d);
  printf("The current year is: %d\n", d.year);
  printf("The current day is: %d\n", d.day);
  printf("The current month is: %d\n", d.month);
  printf("Day of week: %d\n", d.dayofweek);

  _dos_setdate(&orig_d);
  printf("The current year is: %d\n", orig_d.year);
  printf("The current day is: %d\n", orig_d.day);
  printf("The current month is: %d\n", orig_d.month);
  printf("Day of week: %d\n", orig_d.dayofweek);

  return 0;
}
#endif                          /* DEBUG_STANDALONE */

#endif                          /* _NO_DATE */
