/*
	CSWAP.H
*/

#ifdef FEATURE_XMS_SWAP

#ifndef __XMS_SWAP_H
#define __XMS_SWAP_H

#include "../include/misc.h"

/* align one byte */
#pragma option -a-
struct ParamDosExec {
	unsigned short envSeg;
	void far *cmdtail;
	void far *FCB1;
	void far *FCB2;
	void far *overlPtr1;
	void far *overlPtr2;	
	};
/* default alignment */
#pragma option -a.

extern char far dosFCB1[128];			/* must be setup correctly */
extern char far dosFCB2[128];		

extern struct ParamDosExec far dosParamDosExec;

extern unsigned char far dosCMDNAME[128];
/*extern unsigned char far dosCMDTAIL[128];*/
#define dosCMDTAIL  ((char far*)MK_FP(_psp, 0x80))

extern byte far canexit;
extern void far * far termAddr;
extern word far myPID;
extern word far residentCS;
extern word far origPPID;

//extern int far DosEXEC(void);	
extern int far XMSexec(void);	
extern void far terminateFreeCOMHook(void);
extern void far XMSrequest(void);

extern unsigned (far *far XMSdriverAdress)(void);
	
extern far SWAPresidentEnd;          /* label: end of resident code */
extern unsigned short far SwapResidentSize;	  /* paras of the swap handler */	
extern unsigned short far SwapTransientSize;  /* paras of total command */	

struct XMScopy{
        long length;
        short shandle;
        long soffset;
        short dhandle;
        long  doffset;
        } ;

extern struct XMScopy far XMSsave;

/* Handlers in resident portion for external program execution */
extern void far interrupt lowlevel_cbreak_handler();
extern void far interrupt lowlevel_err_handler();
extern void far interrupt autofail_err_handler();
extern void far interrupt lowlevel_int_2e_handler();

/* functions */
word XMSswapmessagesIn(loadStatus *status);
void XMSinit(void);
int XMSisactive(void);
void XMSexit(void);

#endif

#endif
