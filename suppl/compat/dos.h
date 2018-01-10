/* dos.h compatibility header */

#ifndef __DOS_H
#define __DOS_H

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
