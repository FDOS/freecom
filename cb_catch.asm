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
;   Revision 1.1.1.1.4.2  2001/02/18 21:08:23  skaus
;   add: command WHICH
;   fix: BUILD.BAT and accompanying makefiles for TC++ v1.01
;
;   Revision 1.1.1.1.4.1  2001/02/18 17:59:36  skaus
;   bugfix: KSSF: restore parentPSP on exit
;   chg: Using STRINGS resource for all non-interactive messages
;   chg: moving all assembly files to NASM
;

%include "model.inc"
%include "stuff.inc"

segment _DATA
	EXTERN _ctrlBreak

segment _TEXT
	GLOBAL _initCBreak
	GLOBAL _cbreak_handler

_initCBreak:
	;; At this point DS is the segment of _ctrlBreak
	mov WORD [CS:?freecomSegment], ds
	ret

?freecomSegment DW 0

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
		push ds
		mov ds, [CS:?freecomSegment]
		inc WORD [_ctrlBreak]
		pop ds

recurs:
		clc			;; tell DOS to proceed
		retf 2

%ifdef DEBUG
strBeg:
	db 0dh, 0ah, 'COMMAND: ^Break detected.  ', 0dh, 0ah, 0dh, 0ah, '$'
strEnd db 1
%endif

