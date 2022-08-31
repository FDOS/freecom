/* dos.h compatibility header */

#ifndef __DOS_H
#define __DOS_H

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
extern void _intrf(int nr, union REGPACK *r) __attribute__((regparmcall));

static inline unsigned getpsp(void)
{
	unsigned psp, scratch;
	asm volatile("int $0x21" :
		     "=a"(scratch), "=b"(psp) :
		     "Rah"((unsigned char)0x62) :
		     "cc");
	return psp;
}
#define _psp getpsp()

static inline unsigned char getosmajor(void)
{
	unsigned char osmajor, scratch;
	asm volatile("int $0x21" :
		     "=Ral"(osmajor), "=Rah"(scratch) :
		     "1"((unsigned char)0x30) :
		     "bx", "cx", "cc");
	return osmajor;
}
#define _osmajor getosmajor()

#define FP_OFF(fp)	((unsigned)(fp))
#define FP_SEG(fp)	((unsigned)((unsigned long)(void __far *)(fp)>>16))

#endif
