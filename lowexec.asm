;
;  LOWEXEC.ASM
;
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
;

	INCLUDE MODEL.DEF
	CODESEG

	PUBLIC lowLevelExec

lowLevelExec PROC
	push    bp
	mov     bp, sp
	push    si
	push    di
	push    ds

	lds     dx, [bp+4+2*@CodeSize]      ; load file name
	les     bx, [bp+8+2*@CodeSize]      ; load parameter block
	mov     ax, 4b00h

	mov     Word Ptr cs:[saveSP], sp
	mov     Word Ptr cs:[saveSS], ss
	int     21h
	cli						;; Can be removed for post-8086 CPUs
	mov     ss, Word Ptr cs:[saveSS]
	mov     sp, Word Ptr cs:[saveSP]
	sti

	jc      exec_error   ; if there was an error, the error code is in AX
	xor     ax, ax       ; otherwise, clear AX

exec_error:
	pop     ds
	pop     di
	pop     si
	pop     bp
	ret

saveSP dw ?
saveSS dw ?

lowLevelExec endp

	end
