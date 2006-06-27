; $Id$
;	Int2e handler for XMS Swap
;	variant
; int process_input(int xflag, char *commandline)

segment _TEXT class=CODE

extern _residentCS, _mySS, _mySP, _XMSsave, _XMSdriverAdress, _SwapTransientSize, _my2e_parsecommandline, SWAPXMSdirection, _SwapResidentSize
%define callXMS     call far [cs:_XMSdriverAdress]
%define currentSegmOfFreeCOM    _XMSsave+8

    global myfar2e_parsecommandline
myfar2e_parsecommandline:
    call _my2e_parsecommandline
    retf

	global _lowlevel_int_2e_handler
_lowlevel_int_2e_handler:
        cld
        push si
;       lss sp,[_mySP]
        mov ss,[cs:_mySS]
        mov sp,[cs:_mySP]
;       mov cs,[_residentCS]
;       mov ds,sp
;       mov es,ds
;       mov ds,ss
        mov ah,48h       ; move into conventional memory
        mov bx,[cs:_SwapTransientSize]
        int 21h
        jc swaperr
        mov bx,ax
        sub bx,[cs:currentSegmOfFreeCOM]
        push bx
        mov [cs:currentSegmOfFreeCOM],ax
        call SWAPXMSdirection
        mov ah,0bh
        mov si,_XMSsave
        callXMS
;        call SWAPXMSdirection
        pop bx
        cmp ax,1
        jnz swaperr
;       mov ax,[_mySS]
;       add ax,bx
;       mov ss,ax
;       mov sp,[_mySP]
%if 0
        mov ax,[_mySS]
        sub ax,10
        mov ss,ax
        mov sp,[_mySP]
%endif
       ; calling stuff here
;       push es
        push ds
        pop si
        push si
        mov ds,sp
;       push word 1
;       call _process_input
        call far [myfar2e_parsecommandline]
%if 0
        push ax
        mov ah,0bh       ; move into XMS again
        mov si,_XMSsave
        callXMS
        call SWAPXMSdirection
        pop ax
%endif
        jmp finish
swaperr:
        mov ax,0FFFFh
finish:
        iret
