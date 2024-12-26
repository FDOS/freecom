/*
	CSWAP.H
*/

#ifdef FEATURE_XMS_SWAP

#ifndef __XMS_SWAP_H
#define __XMS_SWAP_H

#include "../include/misc.h"

#include "algnbyte.h"
struct ParamDosExec {
	unsigned short envSeg;
	void far *cmdtail;
	void far *FCB1;
	void far *FCB2;
	void far *overlPtr1;
	void far *overlPtr2;	
	};
#include "algndflt.h"

extern char far dosFCB1[128];			/* must be setup correctly */
extern char far dosFCB2[128];		

extern struct ParamDosExec far dosParamDosExec;

extern unsigned char far dosCMDNAME[128];
/*extern unsigned char far dosCMDTAIL[128];*/
#define dosCMDTAIL  ((char far*)MK_FP(_psp, 0x80))

extern int cdecl XMSexec(void);	

#ifdef __WATCOMC__
typedef unsigned long xmsfunc(unsigned request, unsigned dx, void *si);
#pragma aux xmsfunc = __parm [__ax] [__dx] [__si] __modify [__bx __cx]
extern xmsfunc far *far XMSdriverAdress;
#elif defined(__GNUC__)
extern unsigned far *far XMSdriverAdress;
static inline unsigned long XMSrequest(unsigned request, unsigned dx, void *si)
{
	long ret;
	/* N.B. the XMS driver may clobber %bx even if the call is successful,
	   so we need to mark %bx as clobbered.  Also include %cx, the flags,
	   and main memory in the clobber list, for good measure.
		-- tkchia 2018/08/24 */
	asm volatile("lcall *%%cs:XMSdriverAdress" :
		     "=A"(ret) :
		     "a"(request), "d"(dx), "S"(si), "Rds"(FP_SEG(si)) :
		     "bx", "cx", "cc", "memory");
	return ret;
}
#else
extern unsigned (far *far XMSdriverAdress)(void);
extern void far XMSrequest(void);
#endif
	
extern unsigned char far SWAPresidentEnd;          /* label: end of resident code */
extern unsigned short SwapResidentSize;	  /* paras of the swap handler */	
extern unsigned short far SwapTransientSize;  /* paras of total command */	

struct XMScopy{
        long length;
        short shandle;
        long soffset;
        short dhandle;
        long  doffset;
        } ;

extern struct XMScopy XMSsave;
extern struct XMScopy far XMSrestore;

/* Handlers in resident portion for external program execution */
extern void far ASMINTERRUPT lowlevel_cbreak_handler();
extern void far ASMINTERRUPT lowlevel_err_handler();
extern void far ASMINTERRUPT autofail_err_handler();
extern void far ASMINTERRUPT lowlevel_int_2e_handler();

/* functions */
word XMSswapmessagesIn(loadStatus *status);
void XMSinit(void);
int XMSisactive(void);
void XMSexit(void);

#endif

#endif
