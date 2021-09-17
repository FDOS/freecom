; $Id$
; File:
;                          cswap.asm
; Description:
;                       command.com swapping
;
;                    Copyright (c) 2001 tom ehlert
;                       All Rights Reserved
;
; This file is part of DOS-C.
;
; DOS-C is free software; you can redistribute it and/or
; modify it under the terms of the GNU General Public License
; as published by the Free Software Foundation; either version
; 2, or (at your option) any later version.
;
; DOS-C is distributed in the hope that it will be useful, but
; WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
; the GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public
; License along with DOS-C; see the file COPYING.  If not,
; write to the Free Software Foundation, 675 Mass Ave,
; Cambridge, MA 02139, USA.
;
;

%include "model.inc"
%include "stuff.inc"

segment _BSS 			; transient data (in DS)

 	cglobal SwapResidentSize
SwapResidentSize  resw 1

	cglobal XMSsave
XMSsave	resw 8
%define currentSegmOfFreeCOMsave	XMSsave+8

execSS resw 1
execSP resw 1

segment _DATA

resize_free db 4ah

segment _TEXT

%ifidn __OUTPUT_FORMAT__,elf ; GCC/ELF: no seg so use linker script symbol
extern _TEXTSEG
%else
%define _TEXTSEG _TEXT
%endif

	cglobal dosFCB1,dosFCB2
dosFCB1 times 37 db 0
dosFCB2 times 37 db 0

;;	global _dosCMDTAIL, _dosCMDNAME		use command line from within PSP
	cglobal dosCMDNAME
;;_dosCMDTAIL  times 128 db 0
dosCMDNAME times 128 db 0
		times 256	db 0
    global localStack
localStack:

	cglobal dosParamDosExec
dosParamDosExec times 22	db 0


	cglobal XMSdriverAdress
XMSdriverAdress dd 0
%define callXMS		call far [XMSdriverAdress]

 	cglobal SwapTransientSize
SwapTransientSize  dw 0

	cglobal XMSrestore
XMSrestore	times 8 DW 0
%define xms_handle	XMSrestore+4
%define currentSegmOfFreeCOM	XMSrestore+14

	cglobal canexit
canexit	DB 0		; 1 -> can exit; _else_ --> cannot exit
filler  DB 0

    cglobal mySS, mySP
mySS DW 0
mySP DW 0

execRetval dw 0

real_XMSexec:
		int 21h	; shrink/free: first thing done from resident code

		; do exec


;;		mov ax, cs
                   		; ds:dx = ASCIZ program name
;;		mov ds, ax
		mov  dx,dosCMDNAME
		       
                        ; es:bx = parameter block
		mov es, cx
		mov bx, dosParamDosExec


						; our temporary stack
;;        mov ss, ax
;;        mov  sp,localStack


		mov ax,04b00h		; load & execute
		int 21h

		jc  exec_error
		xor ax,ax


exec_error:
		cld					; don't rely on that

	; reload into memory

        mov cx, cs
        mov ss, cx
        mov  sp,localStack-6	; location on stack of return cs:ip and ds

		; restore:

        mov ds, cx
		mov [execRetval],ax
							; we need some memory

		;; First ensure that FreeCOM is reloaded in low memory
		;; so that LOADHIGH would come into problems
;;ska		mov ax, 5800h		; Get current allocation strategy
;;ska		int 21h
;;ska		mov dx, ax

;;ska		mov ax, 5801h		; Set current allocation strategy
;;ska		mov bx, 0			; low memory / first fit
;;ska		int 21h
		; ignore any errors

		mov ah,48h
		mov bx,[SwapTransientSize]
		int 21h

;;ska		pushf

		; Restore Alloc Strat
;;ska		mov ax, 5801h		; Set current allocation strategy
;;ska		mov bx, dx
;;ska		int 21h

;;ska		popf
		jc DOS_trouble_while_swapping_in

                                ; calculate relocation factor

		mov bx,ax				; new execute address
		sub bx,[currentSegmOfFreeCOM]		; new address - old address
		push bx					;
		mov  [currentSegmOfFreeCOM],ax	; new prog address

								; restore everything to XMS
		mov ah,0bh
		mov si,XMSrestore
		callXMS

		pop bx                  ; get relocation factor back

		cmp ax,1
		jnz XMS_trouble_while_swapping_in

		mov bp,sp
		add [bp+2],bx		; relocate return segment

		retf						; done

XMS_err 		db 'XMS copy error$'
DOS_err 		db 'Memory allocation error$'
common_error	db 0dh,0ah,0ah,'FreeCOM: XMSSwap-In: $'

XMS_trouble_while_swapping_in:
	mov bx,XMS_err
	jmp short trouble_while_swapping_in

DOS_trouble_while_swapping_in:
	mov bx,DOS_err

trouble_while_swapping_in:
;		push cs							; do some error message
;		pop  ds
		mov dx, common_error
		mov ah,09
		int 21h

		mov dx,bx
		mov ah,09
		int 21h

terminate_myself:
		mov ax,04cffh					; and die ...
		int 21h

		;; FALL THROUGH for elder FreeCOM kernels that simply ignore
		;; DOS-4C for shells
		cextern terminateFreeCOMHook
		jmp terminateFreeCOMHook

	global xms_kill
xms_kill:
	; Kill the XMS memory block
	mov dx, [xms_handle]
	or dx, dx
	jz terminate_myself		; no block to deallocate
	mov ah, 0ah				; deallocate XMS memory block
	callXMS
	ret

;
; as I don't know how to set the old interrupt handler
; I write my own
; or we use the TURBO_C _restorezero() and use the
; DOS default handler for that

%if 0
MsgZerodivide db 'integer zero divide$'
	global _ZeroDivideInterrupt
_ZeroDivideInterrupt:

		push cs
		pop ds
		mov dx,MsgZerodivide
		mov ah,09
		int 21h

        mov ax,04c7fh       ; terminate with errorlevel 127
        int 21h
		jmp _ZeroDivideInterrupt
%endif


;********************************************************************
; *************   END OF RESIDENT AREA ******************************
;********************************************************************
	cglobal SWAPresidentEnd
SWAPresidentEnd:

%if 0
;
; normal EXEC
;

		global _DosEXEC
_DosEXEC:
						; save ALL registers needed later
		push si
		push di
		push bp
		push ds

		push cs                   		; ds:dx = ASCIZ program name
		pop	 ds

		mov  dx,_dosCMDNAME

		mov [execSS],ss
		mov [execSP],sp

		push cs
		pop  es
		mov bx, _dosParamDosExec

		mov ax,04b00h
		int 21h

		jc      exec_error2  ; if there was an error, the error code is in AX

		xor ax,ax

exec_error2:

		cld					; don't rely on that

		mov ss,[cs:execSS]
		mov sp,[cs:execSP]

		pop ds
		pop bp
		pop di
		pop	si
		retf
%endif

;; Added here to make it more easier for the C-part to make a XMS
;; request, because the code:
;;		(*XMSdriverAdress)();
;; is translated into something like:
;;		mov ax, _CODE		;; immediate value
;;		mov es, ax
;;		call DWORD PTR es:[_XMSdriverAdress]
;; detroying AX already holding the API function number

;; To be called with _far_!!
	cglobal XMSrequest
	;; Note: Because [CS:driverAdress] == [residentCS:driverAdress]
	;; we need not use a similiar approach as with XMSexec
XMSrequest:
		jmp far [cs:XMSdriverAdress]

;; Added here to make it more easier for the C-part to call functions
;; located in the resident part, because:
;;		_XMSexec();
;; is translated into something like:
;;		nop
;;		push cs			<-> WRONG!!
;;		call _XMSexec
;; now also used to contain code that does not need to be resident.

;;TODO: DS ought to be equal to SS, DS could be reconstructed from
;;	SS at the end of the XMSexec function
		cglobal	XMSexec
XMSexec:
						; save ALL registers needed later
%ifidn __OUTPUT_FORMAT__,elf 	; GCC: need to preserve es
		push es
%endif
		push si
		push di
		push bp

		mov [execSS],ss
		mov [execSP],sp

						; save everything to XMS
		mov ah,0bh
		mov si,XMSsave
		call far [cs:XMSdriverAdress]

;;TODO: test of result

		mov es,[currentSegmOfFreeCOMsave]
						; first time: shrink current psp
		mov ah,[resize_free]
		mov bx,[SwapResidentSize]

		mov dx, ds
		mov cx, _TEXTSEG
reloc1 equ $-2
		mov ds, cx

        mov [mySS],ss  ; 2E
        mov [mySP],sp  ; 2E

		mov ss, cx		; this stack is definitely large enough AND present
        mov  sp,localStack

		push dx			; save DS of transient portion
		call _TEXTSEG:real_XMSexec
reloc2 equ $-2

ret_from_resident:
		mov ax,[execRetval]

                                    ; relocate segment registers
		add [bp+4],bx				; ds
		pop ds

		mov byte [resize_free],49h ; change to "free" for next times

		add [currentSegmOfFreeCOMsave],bx
		mov cx,[execSS]
		add cx,bx
		mov ss,cx
		mov sp,[execSP]

%ifidn MODEL, m                     ; in medium & large need to fixup return segment
		mov bp, sp
		add [bp+8],bx
%endif
		pop bp
		pop di
		pop	si
%ifidn __OUTPUT_FORMAT__,elf 	; GCC: need to preserve es
		pop es
%endif
		ret							; done (really), retn/retf based on memory model, see model.inc

%ifidn __OUTPUT_FORMAT__,elf
; NASM does not support segment relocations so add them
; manually
segment .msdos_mz_reloc
		dw reloc1, _TEXTSEG
		dw reloc2, _TEXTSEG
%endif
