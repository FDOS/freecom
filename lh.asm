        TITLE   LH.ASM

; This file contains DOS interface functions used by LOADHIGH.C

DGROUP  GROUP   _DATA
        ASSUME  CS:_TEXT, DS:DGROUP

_DATA   SEGMENT WORD PUBLIC 'DATA'
        extrn   _umbLink:WORD           ; Holds the current state of the UMB link
_DATA   ENDS

_TEXT   SEGMENT WORD PUBLIC 'CODE'

        public  _DosSetUMBLink, _DosSetStrategy
        public  _DosAlloc, _DosResize, _DosFree
;        extrn  _DosAlloc, _DosResize, _DosFree
        public  _farmemcmp, _GetFirstMCB

frame   equ     [bp+4]          ; points to the stack frame

; Most of these functions are just an interface between a C program and
; the int 21h API. They load the registers from their parameters, and makes
; an int 21h call.

_DosAlloc       PROC NEAR   ; Allocate a DOS memory block
        push    bp
        mov     bp, sp

        mov     ah, 48h
        mov     bx, frame
        call    calldos
        jnc     alloc_ok
        xor     ax, ax

alloc_ok:
        pop     bp
        ret
_DosAlloc ENDP

; DosSetUMBLink:
;
; Set the UMB link. The global variable UMBLink will reflect
; the new state of the UMB link. The return value is the old
; state of the UMB link.

_DosSetUMBLink PROC NEAR
        push    bp
        mov     bp, sp
        push    si

        mov     ax, 5802h       ; get UMB link
        call    calldos
        mov     si, ax          ; save the old state
        and     si, 0ffh

        mov     ax, 5803h       ; set UMB link
        mov     bx, word ptr frame
        call    calldos
        mov     ax, frame
        jnc     yup
        xor     ax, ax

yup:    mov     DGROUP:_umbLink, ax
        mov     ax, si

        pop     si
        pop     bp
        ret
_DosSetUMBLink ENDP

_DosSetStrategy PROC NEAR
; set the new malloc strategy,  and return the old

        push    bp
        mov     bp, sp
        push    si

        mov     ax, 5800h
        call    calldos
        mov     si, ax          ; save the old strategy

        mov     ax, 5801h
        mov     bx, word ptr frame      ; new strategy
        call    calldos

        mov     ax, si
        pop     si
        pop     bp
        ret
_DosSetStrategy ENDP

_DosFree PROC NEAR
; free a memory block
        push    bp
        mov     bp, sp

        mov     es, word ptr frame    ; memory block to free
        mov     ah, 49h
        call    calldos

        pop     bp
        ret
_DosFree ENDP

_DosResize PROC NEAR
; resize a memory block

        push    bp
        mov     bp, sp

        mov     es, word ptr frame     ; memory block to resize
        mov     bx, word ptr frame+2   ; new size
        mov     ah, 4ah
        call    calldos
        jc      resize_err
        xor     ax, ax

resize_err:
        pop     bp
        ret
_DosResize ENDP

_GetFirstMCB PROC
; Get start of MCB chain, from word ptr SYSVARS[-2]

        mov     ah, 52h
        call    calldos
        mov     ax, word ptr es:[bx-2]
        ret
_GetFirstMCB ENDP

_farmemcmp PROC NEAR

; Does a simple compare between two far memory pointers.
; Returns zero if they're equal, non-zero if not.

        push    bp
        mov     bp, sp
        push    ds
        push    si
        push    di

        lds     si, frame
        les     di, frame+4
        mov     cx, frame+8
        xor     ax, ax
        repe    cmpsb
        je      get_out

        dec     ax

get_out:
        pop     di
        pop     si
        pop     ds
        pop     bp
        ret
_farmemcmp ENDP

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


