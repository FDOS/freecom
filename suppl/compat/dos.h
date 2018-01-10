/* dos.h compatibility header */

#ifndef __DOS_H
#define __DOS_H

#include <unistd.h>
#define delay(x) usleep(x*1000UL)

static inline unsigned char inportb (unsigned short int port)
{
	unsigned char v;

	asm volatile ("inb %w1,%0":"=a" (v):"Nd" (port));
	return v;
}

static inline void outportb (unsigned short int port, unsigned char value)
{
	asm volatile ("outb %b0,%w1": :"a" (value), "Nd" (port));
}

/* From LGPL DJGPP libc, Copyright (C) 1995 DJ Delorie */
static inline void sound(int freq)
{
	int scale = 1193046 / freq;
	outportb(0x43, 0xb6);
	outportb(0x42, scale & 0xff);
	outportb(0x42, scale >> 8);
	outportb(0x61, inportb(0x61) | 3);
}
static inline void nosound(void)
{
	outportb(0x61, inportb(0x61) & ~3);
}

union REGPACK {
	struct {
		unsigned char al, ah, bl, bh, cl, ch, dl, dh;
	} h;
	struct {
		unsigned short ax, bx, cx, dx, bp, si, di, ds, es, flags;
	} x;
};
extern void intr(int nr, union REGPACK *r, ...);

static inline unsigned getpsp(void)
{
	unsigned psp;
	asm volatile("int $0x21" :
		     "=b"(psp) :
		     "Rah"((unsigned char)0x62) :
		     "ax");
	return psp;
}
#define _psp getpsp()

static inline unsigned char getosmajor(void)
{
	unsigned char osmajor;
	asm volatile("int $0x21" :
		     "=Ral"(osmajor) :
		     "Rah"((unsigned char)0x30) :
		     "bx", "cx");
	return osmajor;
}
#define _osmajor getosmajor()

#define FP_OFF(fp)	((unsigned)(fp))
#define FP_SEG(fp)	((unsigned)((unsigned long)(void __far *)(fp)>>16))

#endif
