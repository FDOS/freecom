        TITLE   LH.ASM
; $Id$

; This file contains DOS interface functions used by LOADHIGH.C

; 2001/02/14 ska
;	sub: duplicate functions (memory API, farmemcmp)
;

DGROUP  GROUP   _DATA
        ASSUME  CS:_TEXT, DS:DGROUP

_DATA   SEGMENT WORD PUBLIC 'DATA'
_DATA	ENDS

_TEXT   SEGMENT WORD PUBLIC 'CODE'

        public  _DosSetUMBLink, _DosSetStrategy
        public   _GetFirstMCB

frame   equ     [bp+4]          ; points to the stack frame

; Most of these functions are just an interface between a C program and
; the int 21h API. They load the registers from their parameters, and makes
; an int 21h call.

; DosSetUMBLink:
;
; Set the UMB link. The global variable UMBLink will reflect
; the new state of the UMB link. The return value is the old
; state of the UMB link.

_DosSetUMBLink PROC NEAR
        push    bp
        mov     bp, sp

        mov     ax, 5802h       ; get UMB link
        call    calldos
        mov     dx, ax          ; save the old state

        mov     ax, 5803h       ; set UMB link
        mov     bx, word ptr frame
        call    calldos			;; result ignored

        xor		ax, ax
        mov     al, dl

        pop     bp
        ret
_DosSetUMBLink ENDP

_DosSetStrategy PROC NEAR
; set the new malloc strategy,  and return the old

        push    bp
        mov     bp, sp

        mov     ax, 5800h
        call    calldos
        mov     dx, ax          ; save the old strategy

        mov     ax, 5801h
        mov     bx, word ptr frame      ; new strategy
        call    calldos

        mov     ax, dx
        pop     bp
        ret
_DosSetStrategy ENDP


_GetFirstMCB PROC
; Get start of MCB chain, from word ptr SYSVARS[-2]

        mov     ah, 52h
        call    calldos
        mov     ax, word ptr es:[bx-2]
        ret
_GetFirstMCB ENDP

calldos PROC NEAR

; This routine is used to call DOS.
; Some DOS versions don't always save registers as they should.
; By using this function, this won't be a problem.

        push    si
        push    di
        push    bp

        clc
        int     21h

        pop     bp
        pop     di
        pop     si
        ret
calldos ENDP

_TEXT   ENDS
        END


