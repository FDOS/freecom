; $Id$
;  LOWEXEC.ASM
;
;
;  Comments:
;
;  ??/??/95 (Steffen Kaiser)
;    started.
;
;  12/??/95 (Svante Frey)
;    reorganized the code
;
;  01/06/96 (Tim Norman)
;    added this history
;
;  08/07/96 (Steffen Kaiser)
;    made argument handling independent of memory model
; $Log$
; Revision 1.2  2001/03/07 19:48:59  skaus
; Merged in Swap Support changes
;
; Revision 1.1.1.1.4.1  2001/02/18 17:59:36  skaus
; bugfix: KSSF: restore parentPSP on exit
; chg: Using STRINGS resource for all non-interactive messages
; chg: moving all assembly files to NASM
;

%include "model.inc"
%include "stuff.inc"

segment _TEXT
	GLOBAL _lowLevelExec

_lowLevelExec:
	push    bp
	mov     bp, sp
	push    si, di, ds

	lds     dx, [bp+4+2*@CodeSize]      ; load file name
	les     bx, [bp+8+2*@CodeSize]      ; load parameter block
	mov     ax, 4b00h

	mov     Word [cs:saveSP], sp
	mov     Word [cs:saveSS], ss
	int     21h
	cli						;; Can be removed for post-8086 CPUs
	mov     ss, [cs:saveSS]
	mov     sp, [cs:saveSP]
	sti

	jc      exec_error   ; if there was an error, the error code is in AX
	xor     ax, ax       ; otherwise, clear AX

exec_error:
	pop    si, di, ds
	pop     bp
	ret

saveSP dw 0
saveSS dw 0

	end
