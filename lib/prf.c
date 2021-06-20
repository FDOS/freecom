/****************************************************************/
/*                                                              */
/*                            prf.c                             */
/*                                                              */
/*                  Abbreviated printf Function                 */
/*                                                              */
/*                      Copyright (c) 1995                      */
/*                      Pasquale J. Villani                     */
/*                      All Rights Reserved                     */
/*                                                              */
/* This file is part of DOS-C.                                  */
/*                                                              */
/* DOS-C is free software; you can redistribute it and/or       */
/* modify it under the terms of the GNU General Public License  */
/* as published by the Free Software Foundation; either version */
/* 2, or (at your option) any later version.                    */
/*                                                              */
/* DOS-C is distributed in the hope that it will be useful, but */
/* WITHOUT ANY WARRANTY; without even the implied warranty of   */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See    */
/* the GNU General Public License for more details.             */
/*                                                              */
/* You should have received a copy of the GNU General Public    */
/* License along with DOS-C; see the file COPYING.  If not,     */
/* write to the Free Software Foundation, 675 Mass Ave,         */
/* Cambridge, MA 02139, USA.                                    */
/****************************************************************/

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <../include/misc.h>
#include <fmemory.h>

#define FALSE 0
#define TRUE 1

static char *charp, *pbuf;

static void handle_char(int, FILE *);
static char * ltob(long, char *, int);
static int do_printf(FILE *f, const char *, register va_list);

#ifndef DEBUG
FILE *stdin = (FILE *)0, *stdout= (FILE *)1, *stderr = (FILE *)2;
#endif

static void flushbuf(FILE *f)
{
  dos_write(fileno(f), pbuf, charp - pbuf);
  charp = pbuf;
}

int puts(const char *s)
{
  return printf("%s\n", s);
}

/* special handler to switch between sprintf and printf */
static void handle_char(int c, FILE *f)
{
  if (c == '\n')
    *charp++ = '\r';
  *charp++ = c;
  /* flush buffer for '\n' or if full */
  if (f && (charp - pbuf >= 79 || c == '\n'))
    flushbuf(f);
}

/* ltob -- convert an long integer to a string in any base (2-16) */
char *ltob(long n, char * s, int base)
{
  unsigned long u;
  char *p, *q;
  int c;

  u = n;

  if (base == -10)              /* signals signed conversion */
  {
    base = 10;
    if (n < 0)
    {
      u = -n;
      *s++ = '-';
    }
  }

  p = q = s;
  do
  {                             /* generate digits in reverse order */
    *p++ = "0123456789ABCDEF"[(unsigned short) (u % base)];
  }
  while ((u /= base) > 0);

  *p = '\0';                    /* terminate the string */
  while (q < --p)
  {                             /* reverse the digits */
    c = *q;
    *q++ = *p;
    *p = c;
  }
  return s;
}

#define LEFT    0
#define RIGHT   1

/* printf -- short version of printf to conserve space */
int vprintf(const char * fmt, va_list arg)
{
  return vfprintf(stdout, fmt, arg);
}

int vfprintf(FILE *f, const char * fmt, va_list arg)
{
  char buf[80];
  int ret;
  charp = pbuf = buf;
  ret = do_printf(f, fmt, arg);
  flushbuf(f);
  return ret;
}

int printf(const char * fmt, ...)
{
  va_list arg;

  va_start(arg, fmt);
  return vfprintf(stdout, fmt, arg);
}

int fprintf(FILE *f, const char * fmt, ...)
{
  va_list arg;

  va_start(arg, fmt);
  return vfprintf(f, fmt, arg);
}

int sprintf(char * buff, const char * fmt, ...)
{
  va_list arg;

  va_start(arg, fmt);
  return vsprintf(buff, fmt, arg);
}

int vsprintf(char *buff, const char * fmt, va_list arg)
{
  charp = buff;
  do_printf(NULL, fmt, arg);
  handle_char(0, NULL);
  return charp - buff - 1;
}

int do_printf(FILE *f, const char * fmt, va_list arg)
{
  int base;
  char s[11];
  char far * p;
  int c, flag, size, fill, precision;
  int longarg;
  long currentArg;

  while ((c = *fmt++) != '\0')
  {
    if (c != '%')
    {
      handle_char(c, f);
      continue;
    }

    longarg = FALSE;
    flag = RIGHT;
    fill = ' ';

    if (*fmt == '0')
    {
      fill = '0';
      fmt++;
    }

    if (*fmt == '-')
    {
      flag = LEFT;
      fmt++;
    }

    for(;;)
    {
      if (*fmt == '*')
      {
        precision = va_arg(arg, int);
        fmt++;
      }
      else
      {
        precision = 0;
        while (*fmt >= '0' && *fmt <= '9')
        {
          precision = precision * 10 + (*fmt++ - '0');
        }
      }

      if(c == '%')
      {
        size = precision;
        precision = -1;
        if (size < 0)
        {
          size = -size;
          flag = LEFT;
        }
      }
      if((c = *fmt) != '.')break;
      fmt++;
    }

    if (c == 'l')
    {
      longarg = TRUE;
      fmt++;
    }

    c = *fmt++;
    switch (c)
    {
      case '\0':
        return 0;

      case 'c':  
        handle_char(va_arg(arg, int), f);
        continue;

      case 'p':
        {
          unsigned short w0 = va_arg(arg, unsigned int);
          char *tmp = charp;
          sprintf(s, "%04x:%04x", va_arg(arg, unsigned int), w0);
          p = s;
          charp = tmp;
          goto do_outputstring;
        }

      case 's':
        p = va_arg(arg, char *);
        goto do_outputstring;

      case 'F':
        fmt++;
        /* we assume %Fs here */
      case 'S':
        p = va_arg(arg, char far *);
        goto do_outputstring;

      case 'i':
      case 'd':
        base = -10;
        goto lprt;

      case 'o':
        base = 8;
        goto lprt;

      case 'u':
        base = 10;
        goto lprt;

      case 'X':
      case 'x':
        base = 16;

      lprt:
        if (longarg)
          currentArg = va_arg(arg, long);
        else
          currentArg = base < 0 ? (long)va_arg(arg, int) :
              (long)va_arg(arg, unsigned int);
        ltob(currentArg, s, base);

        p = s;
      do_outputstring:

        size -= _fstrlen(p);

        if (flag == RIGHT)
        {
          for (; size > 0; size--)
            handle_char(fill, f);
        }
        for (; *p != '\0' && precision != 0; p++, precision--)
          handle_char(*p, f);

        for (; size > 0; size--)
          handle_char(fill, f);

        continue;

      case '%':
        handle_char(c, f);
        break;
      default:
        handle_char('?', f);

        handle_char(c, f);
        break;

    }
  }
  va_end(arg);
  return 0;
}

#ifdef TEST
/*
	this testprogram verifies that the strings are printed correctly
	( or the way, I expect them to print)
	
	compile like (note -DTEST !)

	c:\tc\tcc -DTEST -DI86 -I..\hdr prf.c
	
	and run. if strings are wrong, the program will wait for the ANYKEY

*/
#include <conio.h>
void cso(char c)
{
  putch(c);
}

struct {
  char *should;
  char *format;
  unsigned lowint;
  unsigned highint;

} testarray[] = {
  {
  "hello world", "%s %s", (unsigned)"hello", (unsigned)"world"},
  {
  "hello", "%3s", (unsigned)"hello", 0},
  {
  "  hello", "%7s", (unsigned)"hello", 0},
  {
  "hello  ", "%-7s", (unsigned)"hello", 0},
  {
  "hello", "%s", (unsigned)"hello", 0},
  {
  "1", "%d", 1, 0},
  {
  "-1", "%d", -1, 0},
  {
  "65535", "%u", -1, 0},
  {
  "-32768", "%d", 0x8000, 0},
  {
  "32767", "%d", 0x7fff, 0},
  {
  "-32767", "%d", 0x8001, 0},
  {
  "8000", "%x", 0x8000, 0},
  {
  "   1", "%4x", 1, 0},
  {
  "0001", "%04x", 1, 0},
  {
  "1   ", "%-4x", 1, 0},
  {
  "1000", "%-04x", 1, 0},
  {
  "1", "%ld", 1, 0},
  {
  "-1", "%ld", -1, -1},
  {
  "65535", "%ld", -1, 0},
  {
  "65535", "%u", -1, 0},
  {
  "8000", "%lx", 0x8000, 0},
  {
  "80000000", "%lx", 0, 0x8000},
  {
  "   1", "%4lx", 1, 0},
  {
  "0001", "%04lx", 1, 0},
  {
  "1   ", "%-4lx", 1, 0},
  {
  "1000", "%-04lx", 1, 0},
  {
  "-2147483648", "%ld", 0, 0x8000},
  {
  "2147483648", "%lu", 0, 0x8000},
  {
  "2147483649", "%lu", 1, 0x8000},
  {
  "-2147483647", "%ld", 1, 0x8000},
  {
  "32767", "%ld", 0x7fff, 0},
  {
"ptr 1234:5678", "ptr %p", 0x5678, 0x1234}, 0};

test(char *should, char *format, unsigned lowint, unsigned highint)
{
  char b[100];

  sprintf(b, format, lowint, highint);

  printf("'%s' = '%s'\n", should, b);

  if (strcmp(b, should))
  {
    printf("\nhit the ANYKEY\n");
    getch();
  }
}

main()
{
  int i;
  printf("hello world\n");

  for (i = 0; testarray[i].should; i++)
  {
    test(testarray[i].should, testarray[i].format, testarray[i].lowint,
         testarray[i].highint);
  }
}
#endif

