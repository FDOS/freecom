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

/*
 * 07/29/98 (Rob Lake)
 *  started
 *
 * Works Under:
 *    Borland C++ 3.0
 *  Pacific C (pacc 7.51)
 *
 */

#ifdef _NO__DOS_TIME

#include <dos.h>
#include <stdio.h>
#include "timefunc.h"

unsigned _dos_settime(struct dostime_t *t)
{
  union REGS r;

  r.h.ah = 0x2D;
  r.h.ch = t->hour;
  r.h.cl = t->minute;
  r.h.dh = t->second;
  r.h.dl = t->hsecond;

  int86(0x21, &r, &r);

  if (r.h.al == 0xFF)           /* Error occured setting time */
    return -1;

  return 0;
}

void _dos_gettime(struct dostime_t *t)
{
  union REGS r;

  r.h.ah = 0x2C;
  int86(0x21, &r, &r);

  t->hour = r.h.ch;
  t->minute = r.h.cl;
  t->second = r.h.dh;
  t->hsecond = r.h.dl;
}

#ifdef DEBUG_STANDALONE
int main(void)
{
  struct dostime_t new_ct;
  struct dostime_t t;
  struct dostime_t orig_t;

  _dos_gettime(&orig_t);
  printf("The current time is: %2d:%02d:%02d.%02d\n",
         orig_t.hour, orig_t.minute, orig_t.second, orig_t.hsecond);

  new_ct.hour = 12;
  new_ct.minute = 12;
  new_ct.second = 12;
  new_ct.hsecond = 12;

  _dos_settime(&new_ct);

  _dos_gettime(&t);
  printf("The current time is: %2d:%02d:%02d.%02d\n",
         t.hour, t.minute, t.second, t.hsecond);

  _dos_settime(&orig_t);
  printf("The current time is: %2d:%02d:%02d.%02d\n",
         orig_t.hour, orig_t.minute, orig_t.second, orig_t.hsecond);

  return 1;
}
#endif

#endif
