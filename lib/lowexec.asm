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
; Revision 1.1  2001/04/12 00:33:53  skaus
; chg: new structure
; chg: If DEBUG enabled, no available commands are displayed on startup
; fix: PTCHSIZE also patches min extra size to force to have this amount
;    of memory available on start
; bugfix: CALL doesn't reset options
; add: PTCHSIZE to patch heap size
; add: VSPAWN, /SWAP switch, .SWP resource handling
; bugfix: COMMAND.COM A:\
; bugfix: CALL: if swapOnExec == ERROR, no change of swapOnExec allowed
; add: command MEMORY
; bugfix: runExtension(): destroys command[-2]
; add: clean.bat
; add: localized CRITER strings
; chg: use LNG files for hard-coded strings (hangForEver(), init.c)
; 	via STRINGS.LIB
; add: DEL.C, COPY.C, CBREAK.C: STRINGS-based prompts
; add: fixstrs.c: prompts & symbolic keys
; add: fixstrs.c: backslash escape sequences
; add: version IDs to DEFAULT.LNG and validation to FIXSTRS.C
; chg: splitted code apart into LIB\*.c and CMD\*.c
; bugfix: IF is now using error system & STRINGS to report errors
; add: CALL: /N
;
; Revision 1.1.1.1.4.1  2001/02/18 17:59:36  skaus
; bugfix: KSSF: restore parentPSP on exit
; chg: Using STRINGS resource for all non-interactive messages
; chg: moving all assembly files to NASM
;

%include "../include/model.inc"
%include "../include/stuff.inc"

segment _TEXT
	cglobal lowLevelExec

lowLevelExec:
	push    bp
	mov     bp, sp
	pushm   si, di, ds

%ifidn __OUTPUT_FORMAT__, elf
	push    es
%endif
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
%ifidn __OUTPUT_FORMAT__, elf
	pop     es
%endif
	popm    si, di, ds
	pop     bp
	ret

saveSP dw 0
saveSS dw 0
