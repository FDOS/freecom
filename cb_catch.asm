COMMENT ~
   $Id$
   $Locker$	$Name$	$State$

	^Break signal catcher and initialization function.

	It performs two tasks:
		1) If the current process is the same as the one the init
		function has called, only ctrlBreak is set to 1 (one)
		and the ^Break is ignored furtherly.
		2) Otherwise, the process is terminated.

	See the comments of the definitions of DEBUG and LOCAL_PID.

	Note: TASM maintains "@Model" as a numerical representation of the
		memory model, "1" == "Tiny". I don't know, if it is portable,
		but the "SEG _psp" generates a reference EXE2COM cannot
		resolve.

   $Log$
   Revision 1.1  2000/05/20 03:29:33  jimtabor
   Initial revision

   Revision 1.3  2000/03/30 14:07:03  jprice
   Merge in changes from Steffen's 0.78ska2 code

   Revision 1.1  1999/11/04 06:45:07  ska
   Initial revision

   Revision 1.2  1999/09/20 23:56:35  jprice
   Steffen Kaiser patches

   Revision 1.2  1999/07/12 03:17:29  ska
   chg: added v0.77 patches <<what is the print_free() stuff in DIR.C?>>
   chg: centralized DEBUG stuff (see debug.*)
   add: SET: Issues no-such-variable if a variable could not be removed
   bugfix: COPY *.C A:

~
	;; Source in the current model
	INCLUDE MODEL.DEF
	DATASEG

;; Uncomment to enable debugging
;; If debugging is enabled, the catcher displays a line "COMMAND: ..."
;; onto stdout; however, it stdout is redirected, the line would be
;; redirected, too.
; DEBUG EQU 1

	EXTRN ctrlBreak:WORD		;; where to record ^Break to
	EXTRN _psp:WORD				;; COMMAND's process ID, if your C compiler
				;; does not provide this identifier, check for another one
				;; or comment out this definition, but uncomment the following
				;; one
; LOCAL_PID EQU 1

	CODESEG

	PUBLIC initCBreakCatcher

IFDEF LOCAL_PID
?psp	dw ?		;; local process ID, if you want to use the C compiler's
					;; PID variable, you have to EQU this variable to the
					;; C identifier, declare it properly and add the
					;; segment adjustment within the CBreakCatcher
ENDIF

initCBreakCatcher PROC
IFDEF LOCAL_PID
	mov ah, 51h		; Get PID
	int 21h
	mov cs:?psp, bx
ENDIF
	push ds			;; some memory models require this unchanged
	mov ax, cs
	mov ds, ax
	mov dx, OFFSET ?CBreakCatcher
	mov ax, 2523h	;; Set ^Break handler
	int 21h
	pop ds
	ret
initCBreakCatcher ENDP

IFDEF DEBUG
strBeg:
	db 0dh, 0ah, 'COMMAND: ^Break detected.   ', 0dh, 0ah, 0dh, 0ah, '$'
strEnd db 1
ENDIF

?CBreakCatcher:
		push ax
		push ds
		push bx

IFDEF DEBUG
		dec strEnd
		jz noRecurs
		pop bx
		inc strEnd
		clc			;; recursively called ==> ignore
		jmp short recurs

noRecurs:
		push dx
		mov dx, Offset strBeg
		mov ax, cs
		mov ds, ax
		mov ah, 9
		int 21h
		inc strEnd
		pop dx
ENDIF

		mov ah, 51h
		int 21h
IFDEF LOCAL_PID
		cmp cs:?psp, bx
ELSE
IF @Model EQ 1		;; if tiny model
		mov ax, cs
ELSE
		mov ax, SEG _psp
ENDIF
		mov ds, ax
		cmp _psp, bx
ENDIF
		pop bx

		jz localCBreak

		;; ^Break of a called program --> terminate
		pop ds
		pop ax
		stc			;; tell DOS to terminate the program
		retf		;; ret far

localCBreak:
		;; ^Break of COMAMND --> just set the variable
IF @Model EQ 1		;; if tiny model
		mov ax, cs
ELSE
		mov ax, SEG ctrlBreak	;; Note SEG ctrlBreak might != SEG _psp
ENDIF

		mov ds, ax
		mov ctrlBreak, 1

recurs:
		pop ds
		pop ax

;;		clc			;; tell DOS to proceed	; set by CMP above
		retf 2

END
