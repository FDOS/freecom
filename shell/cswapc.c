/* $Id$
 File:
                          cswapc.c
 Description:
                       command.com swapping

                    Copyright (c) 2001 tom ehlert
                       All Rights Reserved


 more description:
 	this allows COMMAND.COM swapping to XMS memory, completely
 	ignoring all poor XT's ;-) (AT's would be handled if HIMEM
 	would handle them)

	command.com is split into 2 parts - the resident part, located
	int CSWAP.ASM, which should be located at or near begin of command.com
	and the transient part.

	during command execution, the transient portion is freed.
	after execution, DOS is asked to give us some memory again,

	the complete image is then copied there, and SS,CS,DS located to the
	new position.

	this works as long as the rest of command.com nowhere references
	_TEXT, _DATA like
		mov ax,_TEXT
		mov ax,DGROUP

	useful to verify this is

	kernel2025c\utils\exeflat command.exe

	which will show, where what segments are referenced

	============

	Structure of XMS memory block:
	offset 		useage
	0			Strings resource
	N*16		FreeCOM

	N == number of paragraphes required to store the STRINGS
	resource into (XMSmsglen).

;
; This file is part of DOS-C.
...

*/

#include "../config.h"

#include <assert.h>
#include <suppl.h>
#include <mcb.h>
#include "../include/command.h"

#include "../include/cswap.h"

/*#define XMSALLOCSIZE 128*/	/* 128Kb available */

#if 0
	#define MK_FP(seg,ofs)	((void far *) \
				   (((unsigned long)(seg) << 16) | (unsigned)(ofs)))

	#define FP_OFF(fp)	((unsigned)(fp))
	#define FP_SEG(fp)	((unsigned)((unsigned long)(fp) >> 16))
#endif

#define RM2PHYS_ADDR(segm,ofs)	(SEG2PHYS_ADDR((segm)) | (ofs))
#define SEG2PHYS_ADDR(segm)		((unsigned long)(segm) << 16)

static word XMSmsglen = 0;
static enum {
	INIT_NO
	,INIT_FAILED
	,INIT_SUCCEEDED
} initialized = INIT_NO;

int XMSisactive(void)
{	return initialized == INIT_SUCCEEDED;
}

#if 0
void fmemcpy(void far *d1, void far *s1, unsigned len)
{
	char far *d=d1, far *s=s1;

	for ( ; len ; --len)
		*d++ = *s++;
}
#endif

static int XMScopy(
        long length, ...
/*      short shandle,
        long soffset,
        short dhandle,
        long  doffset */
        )
{
	assert(XMSdriverAdress);

/*	asm push si;
	asm lea si,length
	asm mov ah,0bh;	*/
	_SI = (unsigned)&length;
	_AH = 0xb;
	XMSrequest();
/*	asm pop si; */

	return _AX;		/* shut up warning */
}


/* #pragma warn -asc */

/* cannot use inline assembly without external assembler in TC++1 */
void XMSinit(void)
{
	unsigned (far *xmsaddr)(void);
	unsigned xmshandle;
	unsigned msglen, segm;
	struct MCB _seg *mcb;
	unsigned /*long*/ xms_block_size; 

	if(initialized != INIT_NO)	/* already checked --> no changed possible */
		return;
	swapOnExec = ERROR;			/* just in case */
	initialized = INIT_FAILED;

	/* Fetch the size of the message segment */
	segm = msgSegment();
	if(segm) {
		mcb = MK_SEG_PTR (struct MCB, SEG2MCB (segm));
		msglen = mcb->mcb_size;
	} else {
		msglen = 0;		/* do not load messages into XMS?? */
	}

/*   asm     mov ax, 4300h; */
/*   asm     int 2fh; */                /*  XMS installation check */
	_AX = 0x4300;
	geninterrupt(0x2f);

	if(_AL != 0x80)			/* No XMS driver installed */
		return;
/*   asm     cmp al, 80h;		*/
/*   asm     jne not_detected;	*/

/*   asm     mov ax, 4310h; */          /*  XMS get driver address */
/*   asm     int 2fh; */
	_AX = 0x4310;
	geninterrupt(0x2f);

	xmsaddr = MK_FP(_ES, _BX);
/*   asm     mov word ptr xmsaddr, bx;	*/
/*   asm     mov word ptr xmsaddr+2, es;	*/
	if(!xmsaddr)		return;		/* sanity check */

	SwapResidentSize =
		 FP_SEG(&SWAPresidentEnd)
		 + (FP_OFF(&SWAPresidentEnd) + 0x0f) / 16
		 - _psp + 1;

	mcb = MK_SEG_PTR (struct MCB, SEG2MCB (_psp));
	xms_block_size = SwapTransientSize = mcb->mcb_size;

	xms_block_size += msglen;		/* both messages and FreeCOM into
										the XMS block */
/*	xms_block_size *= 16; */			/* the sizes are in paragraphes */
/*	xms_block_size /= 1024; */
	xms_block_size /= 1024 / 16;		/* total size in KB */

/*	asm	mov ah, 09	*/
/*	asm mov dx, XMSALLOCSIZE; */		/* will do for first try */

	_DX = xms_block_size + 1;
	_AH = 9;

	(*xmsaddr)();

	if(_AX) {			/* Got the XMS block */
/*	asm or ax,ax			*/
/*	asm jz not_detected			*/

/*		asm mov xmshandle,dx	*/
		xmshandle = _DX;

		XMSsave.length = SwapTransientSize * 16l;
		XMSdriverAdress = xmsaddr;
/*		XMSsave.shandle = 0;			default value */
/*		XMSsave.soffset = (long)MK_FP(_psp,0); */
		XMSsave.soffset = SEG2PHYS_ADDR(_psp);
		XMSsave.dhandle = xmshandle;
/*		XMSsave.doffset = 0;			default value */
		XMSsave.doffset = msglen * 16l;	/* STRINGS resource preceeds the
											FreeCOM swap area */

		swapOnExec = FALSE;				/* to swap is allowed now */
		defaultToSwap = TRUE;			/* make it the default for XMSwap */
		initialized = INIT_SUCCEEDED;

		/* Unregister the Division by zero handler installed by the
			C startup code */
#if 0
		{
		extern void interrupt far ZeroDivideInterrupt();
		setvect(0,ZeroDivideInterrupt);
        }
#endif
        {
        extern void _restorezero(void);
        _restorezero();
        }

		if(msglen
			/* Because the STRINGS resource never changes,
				it is copied statically into the XMS mem block */
		 && XMScopy(
				msglen * 16l,
				0,							/* from DOS */
				SEG2PHYS_ADDR(segm),		/* message segment */
				XMSsave.dhandle,			/* our common XMS handle */
				0l  /* at beginning of the allocated blk */
		  )) {
			XMSmsglen = msglen;
#ifdef DEBUG
			if(persistentMSGs)
				dbg_outsn("MEM: active XMS Swap superceeds persistent messages");
#endif
			persistentMSGs = FALSE;	/* when the messages are in XMS, there
										is no need to keep them in
										convential memory */
		}
	}

/*not_detected:			*/
/*	;			*/
}



void XMSexit(void)
{
/*	unsigned (far *xmsaddr)(void) = XMSdriverAdress; */
/*	unsigned handle = XMSsave.dhandle; */

	if(initialized == INIT_SUCCEEDED) {
		/* asm     mov dx, XMSsave.dhandle;
		asm     mov ah, 0ah;   			/* free XMS memory */
		_DX = XMSsave.dhandle;
		_AH = 0xa;   			/* free XMS memory */
		XMSrequest();
	}

}

#if 0
	/* Joined with XMSinit() in order to allocate a block
		of memory large enough ska*/
void InitSwapping(void)
{

	if (initialized)
		return;

	initialized = 1;

		XMSinit();

		SwapResidentSize =
				  FP_SEG(&SWAPresidentEnd) +
				 (FP_OFF(&SWAPresidentEnd) +0x0f) / 16 -
				 _psp ;

        SwapTransientSize = *(short far *)MK_FP(_psp-1,3);

		XMSsave.length = (unsigned long)SwapTransientSize * 16;
/*		XMSsave.shandle = 0;					by init */
		XMSsave.soffset = (long)MK_FP(_psp,0);
/*		XMSsave.dhandle = XMShandle;				above	*/
/*		XMSsave.doffset = 0;					by init */

		/* printf("resident  bytes %lu\n",	(long)SwapResidentSize*16);  */
		/* printf("transient bytes %lu\n",	(long)SwapTransientSize*16); */
}
#endif


#if 0
	/* Joined with XMSinit ska*/
void XMSswapmessagesOut(void)
{

  InitSwapping();

  if(msgSegm == 0)
  	return;

  if(XMSdriverAdress == 0) return;

  XMSmsglen = *(word far *)MK_FP(msgSegm-1,3);

  XMScopy(
  		XMSmsglen * 16l,
  		0,							/* from DOS */
  		(long)MK_FP(msgSegm,0),		/* message segment */
		XMSsave.dhandle,			/* our common XMS handle */
  		(long)XMSALLOCSIZE*1024l - XMSmsglen  /* at end off allocated blk */
  		);
  
    freeBlk(msgSegm);
    msgSegm = 0;
}
#endif

word XMSswapmessagesIn(loadStatus *status)
{	word segm;

	assert(status);

	/* Strings in XMS at all? */
	if(!XMSmsglen)	return 0;

	assert(XMSdriverAdress);

	/* Allocate swap buffer in conventional memory
		mode: last fit, high first */
	segm = DOSalloc(XMSmsglen, 0x82);
	if(!segm) {
		*status = STRINGS_OUT_OF_MEMORY;
		return 0;
	}

	if(XMScopy(
	   XMSmsglen * 16l
	  ,XMSsave.dhandle			/* our common XMS handle */
	  ,0l  					/* at beginning of allocated blk */
	  ,0					/* to DOS */
	  ,SEG2PHYS_ADDR(segm)		/* message segment */
	)) {
		*status = STRINGS_LOADED;
		return segm;
	}

	DOSfree(segm);
	return 0;
}

#if 0

DoExec(char *command,char *cmdtail)
{
	int len;
	short retval;

	/* printf("executing %s - %s\n",command, cmdtail); */

	dosParamDosExec.envSeg = 0;

	dosParamDosExec.cmdtail = &dosCMDTAIL;
	dosParamDosExec.cmdtail = &dosCMDTAIL;


	_fmemcpy(dosCMDTAIL+1, cmdtail,128-1);

	len = strlen(cmdtail);
	if (len >= 127) len = 127;
	dosCMDTAIL[0] = len;
	dosCMDTAIL[1+ len] = '\r';

	_fmemcpy(dosCMDNAME, command,128);

	if(initialized == INIT_SUCCEEDED)
		retval = DosEXECSwap();
	else
		retval = DosEXEC();

	return retval;
}
#endif


#define FREECOM_NEED_EXIT
#ifdef __BORLANDC__
#if __BORLANDC__ >= 0x500
#undef FREECOM_NEED_EXIT
#endif
#endif


#ifdef FREECOM_NEED_EXIT
/* Using the original exit() function crashes in TC++ v1.01 */
void exit()
{
	USEREGS
	extern void exitfct(void);	/* from INIT.C */

	exitfct();					/* restore the old owner_psp */

	_AX = 0x4c00;
	geninterrupt(0x21);
}
#endif
