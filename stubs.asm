; $Id$
;
;		Resident portion
;
;	This code:
;	1) provides the stubs for the shared Criter and ^Break modules
;		by preloading the registers ES:BX with the address to
;		the current context
;	2) provides the stub and/or code to reload the transient portion
;	3) is the INT-2E server (not implemented, yet)
;	4) provides space for static context
;	5) contains the PSP of the resident portion (env seg, file descriptors,
;		input buffer for respawner)
;
;	See DEBUG below
;
;	Because the resident portion is FreeCOM as far as other processes shall
;	see it, it is a full-featured process including PSP. This is also the
;	very place where the file descriptors are saved.
;
; $Log$
; Revision 1.1.2.1  2000/12/17 21:57:36  skaus
; intermediate update 1
;

;; Uncomment the following define in order to let the respawner documents
;; its actions on screen
DEBUG EQU 1

MODULE_VERSION EQU 1

ORG 100h

???start:

%include "resource.inc"
%include "context.inc"

?context:
%include "context.def"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;; the stubs

???stub_int24:		;; INT-24 stub
	push es
	push bx			; destroyed to pass forth the context pointer
	mov bx, cs
	mov es, bx
	mov bx, ??int24
	jmp word 0:0
????ref_criter:

???stub_cBreak:		;; ^Break stub
	push es
	push bx			; destroyed to pass forth the context pointer
	mov bx, cs
	mov es, bx
	mov bx, ??cb_catch
	jmp word 0:0
????ref_cbreak:

???stub_respawner:		;; ^Break stub
	jmp word 0:0
????ref_respawner:

???ende:

	resIdBlock RES_ID_RESPAWNER, 0

END
