;   $Id$
;   $Locker$	$Name$	$State$

;	^Break signal catcher and initialization function.
;	Bases on CB_CATCH.ASM
;	It is activated only, if FreeCOM is active.
;	--> FreeCOM polls the ^Break state actively, so the ^C press is
;		recorded, but ignored otherwise

;	There are several possibility to communicate with FreeCOM, because
;	FreeCOM actively polls the ^Break status, this implementation
;	optimizes for the polling rather this handler.

;   $Log$
;   Revision 1.2  2002/04/02 18:09:31  skaus
;   add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
;
;   Revision 1.1  2001/04/12 00:09:06  skaus
;   chg: New structure
;   chg: If DEBUG enabled, no available commands are displayed on startup
;   fix: PTCHSIZE also patches min extra size to force to have this amount
;      of memory available on start
;   bugfix: CALL doesn't reset options
;   add: PTCHSIZE to patch heap size
;   add: VSPAWN, /SWAP switch, .SWP resource handling
;   bugfix: COMMAND.COM A:\
;   bugfix: CALL: if swapOnExec == ERROR, no change of swapOnExec allowed
;   add: command MEMORY
;   bugfix: runExtension(): destroys command[-2]
;   add: clean.bat
;   add: localized CRITER strings
;   chg: use LNG files for hard-coded strings (hangForEver(), init.c)
;   	via STRINGS.LIB
;   add: DEL.C, COPY.C, CBREAK.C: STRINGS-based prompts
;   add: fixstrs.c: prompts & symbolic keys
;   add: fixstrs.c: backslash escape sequences
;   add: version IDs to DEFAULT.LNG and validation to FIXSTRS.C
;   chg: splitted code apart into LIB\*.c and CMD\*.c
;   bugfix: IF is now using error system & STRINGS to report errors
;   add: CALL: /N
;
;   Revision 1.1.1.1.4.2  2001/02/18 21:08:23  skaus
;   add: command WHICH
;   fix: BUILD.BAT and accompanying makefiles for TC++ v1.01
;
;   Revision 1.1.1.1.4.1  2001/02/18 17:59:36  skaus
;   bugfix: KSSF: restore parentPSP on exit
;   chg: Using STRINGS resource for all non-interactive messages
;   chg: moving all assembly files to NASM
;

%include "../include/model.inc"
%include "../include/stuff.inc"

;segment _DATA
;	EXTERN _ctrlBreak

segment _TEXT
;	GLOBAL _initCBreak
	GLOBAL _cbreak_handler
	GLOBAL _CBreakCounter

;_initCBreak:
;	;; At this point DS is the segment of _ctrlBreak
;	mov WORD [CS:?freecomSegment], ds
;	ret

;?freecomSegment DW 0
_CBreakCounter DW 0

_cbreak_handler:
%ifdef DEBUG
		dec BYTE [CS:strEnd]
		jz noRecurs
		inc BYTE [CS:strEnd]
		jmp short recurs

noRecurs:
		push ds, dx, ax, bp
		mov dx, strBeg
		mov ax, cs
		mov ds, ax
		mov ah, 9
		int 21h
		inc BYTE [strEnd]
		pop ds, dx, ax, bp
%endif

		;; ^Break of COMAMND --> just set the variable
		inc WORD [CS:_CBreakCounter]

recurs:
		clc			;; tell DOS to proceed
		retf 2

%ifdef DEBUG
strBeg:
	db 0dh, 0ah, 'COMMAND: ^Break detected.  ', 0dh, 0ah, 0dh, 0ah, '$'
strEnd db 1
%endif

