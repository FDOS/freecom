/* Enables the usage of the fast register pseudo vars for non-TURBO
   compilers. That means the port Turbo -> non-Turbo is OK.
   Back not, because the union holds the registers independed on
   real registers.
*/

#ifndef __REGPROTO_H
#define __REGPROTO_H

#ifndef _MICROC_
#include <dos.h>
#endif

#ifdef __TURBOC__
#define USEREGS
#define NOREGS	/* register variables active */
#define _CFLAG (_FLAGS & 1)

#else

#ifdef _MICROC_
#define USEREGS
#define _AL (*(unsigned char *)&_AX_)
#define _AH (((unsigned char *)&_AX_)[1])
#define _BL (*(unsigned char *)&_BX_)
#define _BH (((unsigned char *)&_BX_)[1])
#define _CL (*(unsigned char *)&_CX_)
#define _CH (((unsigned char *)&_CX_)[1])
#define _DL (*(unsigned char *)&_DX_)
#define _DH (((unsigned char *)&_DX_)[1])
#define _AX _AX_
#define _BX _BX_
#define _CX _CX_
#define _DX _DX_
#define _SI _SI_
#define _DI _DI_
#define _DS get_ds()
#define _SS get_ds()
#define _ES get_es()
#define _CS get_cs()
#define _CFLAG (_FLAGS & 1)
extern unsigned _FLAGS;
#define geninterrupt(n)  (_FLAGS = int86(n))

#else
#define USEREGS  union REGS reg ; struct SREGS sreg ;
#define _AH reg.h.ah
#define _AL reg.h.al
#define _BH reg.h.bh
#define _BL reg.h.bl
#define _CH reg.h.ch
#define _CL reg.h.cl
#define _DH reg.h.dh
#define _DL reg.h.dl
#define _AX reg.x.ax
#define _BX reg.x.bx
#define _CX reg.x.cx
#define _DX reg.x.dx
#define _SI reg.x.si
#define _DI reg.x.di
#define _DS sreg.ds
#define _ES sreg.es
#define _SS sreg.ss
#define _CFLAG reg.x.cflag
#define geninterrupt(n)  int86x(n,&reg,&reg,&sreg)

#endif /* _MICROC_ */
#endif /* __TURBOC__ */
/*
   int getkey()
   {
      USEREGS

      _AH = 0x10 ;
      geninterrupt(0x16) ;
      return _AX ;
   }
*/

#ifdef _MICROC_
#define asmLabel(a) a:			/* label inside of asm {} */
#define asmName(a,b) b[bp]		/* variable name not available, use offset */
#define FP_SEG(a) get_ds()	/* only tiny/small available */
#define FP_OFF(a) (a)
#else
#define asmLabel(a) } a: asm {
#define asmName(a,b) a

#if defined(__POWERC) || (defined(__TURBOC__) && !defined(__BORLANDC__))
#define FAR far
#else
#define FAR _far
#endif

#ifndef MK_FP
#define MK_FP(seg,off) ((void FAR *)(((long)(seg) << 16)|(unsigned)(off)))
#endif
#endif

#endif
