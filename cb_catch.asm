;   $Id$
;
;	Dummy ^Break signal catcher
;
;	It performs two tasks:
;		1) If the current process is the same as the one the init
;		function has called, only ctrlBreak is set to 1 (one)
;		and the ^Break is ignored furtherly.
;		2) Otherwise, the process is terminated.
;
;	On return the caller must return to DOS the following way:
;	if(Carry)	retf
;	if(!Carry)	retf 2
;
;	The current context is passed in in ES:BX, the caller must
;	save/restore ES & BX.
;	Used fields:
;		offset 0, byte, ctrlBreak detected
;		offset 1, word, FreeCOM's PSP address of resident portion
;		offset 3, word, FreeCOM's PSP address of transient portion
;
;	See the comments of the definitions of DEBUG.
;
;   $Log$
;   Revision 1.1.1.1.2.1  2000/12/17 21:57:36  skaus
;   intermediate update 1
;
;   Revision 1.1.1.1  2000/05/20 03:29:33  jimtabor
;   The GNU GPL FreeCom Command Shell
;
;   Revision 1.3  2000/03/30 14:07:03  jprice
;   Merge in changes from Steffen's 0.78ska2 code
;
;   Revision 1.1  1999/11/04 06:45:07  ska
;   Initial revision
;
;   Revision 1.2  1999/09/20 23:56:35  jprice
;   Steffen Kaiser patches
;
;   Revision 1.2  1999/07/12 03:17:29  ska
;   chg: added v0.77 patches <<what is the print_free() stuff in DIR.C?>>
;   chg: centralized DEBUG stuff (see debug.*)
;   add: SET: Issues no-such-variable if a variable could not be removed
;   bugfix: COPY *.C A:

;; Uncomment to enable debugging
;; If debugging is enabled, the catcher displays a line "COMMAND: ..."
;; onto stdout; however, it stdout is redirected, the line would be
;; redirected, too.
; DEBUG EQU 1

MODULE_VERSION EQU 1

%include "resource.inc"
%include "context.inc"

???start:

?CBreakCatcher:
		push ax

%ifdef DEBUG
		dec byte [strEnd]
		jz noRecurs
		inc byte [strEnd]
		clc			;; recursively called ==> ignore
		jmp short recurs

noRecurs:
		push ds
		push dx
		mov dx, strBeg
		mov ax, cs
		mov ds, ax
		mov ah, 9
		int 21h
		inc byte strEnd
		pop dx
		pop ds
%endif

		mov ah, 51h
		int 21h
		cmp ax, [es:bx+?cb_catch_psp1]		;; FreeCOM's PSP 
		jz localCBreak
		cmp ax, [es:bx+?cb_catch_psp2]		;; FreeCOM's PSP 
		jz localCBreak

		;; ^Break of a called program --> terminate
		pop ax
		stc			;; tell DOS to terminate the program
		pop bx		;; Restore the context pointer
		pop es		;; saved by the caller
		retf		;; no IRET!

localCBreak:
		;; ^Break of COMAMND --> just set the variable
		mov byte [es:bx+?cb_catch_cntrlBreak], 1

recurs:
		pop ax

;;		clc			;; tell DOS to proceed	; set by CMP above
		pop bx		;; Restore the context pointer
		pop es		;; saved by the caller
		retf 2		;; no IRET!

%ifdef DEBUG
strBeg:
	db 0dh, 0ah, 'COMMAND: ^Break detected.   ', 0dh, 0ah, 0dh, 0ah, '$'
strEnd db 1
%endif

???ende:

resIdBlock RES_ID_CBREAK

END
