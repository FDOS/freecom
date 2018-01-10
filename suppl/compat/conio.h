/* conio.h compatibility header */

#ifndef __CONIO_H
#define __CONIO_H

#include <stdio.h>
#define cputs(x) fputs((x),stdout)

static inline int getch(void)
{
	unsigned char ch;
	asm volatile("int $0x21" :
		     "=Ral"(ch) :
		     "a"(0x700));
	return ch;
}

#endif
