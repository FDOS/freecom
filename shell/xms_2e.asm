; $Id$
;	Int2e handler for XMS Swap
;	variant
; int process_input(int xflag, char *commandline)

segment _TEXT class=CODE

extern _residentCS, _mySS, _mySP, _XMSsave, _XMSdriverAdress, _SwapTransientSize, _my2e_parsecommandline, SWAPXMSdirection, _SwapResidentSize
%define callXMS     call far [_XMSdriverAdress]
%define currentSegmOfFreeCOM    _XMSsave+8

	global _lowlevel_int_2e_handler
_lowlevel_int_2e_handler:
;       push ss
;       push sp
;       push cs
;       lss sp,[_mySP]
;       mov ss,[_mySS]
;       mov sp,[_mySP]
;       mov cs,[_residentCS]
;       mov ds,sp
;       mov es,ds
;       mov ds,ss
       mov ah,48h       ; move into conventional memory
       mov bx,[_SwapTransientSize]
       int 21h
       jc swaperr
       mov bx,ax
       sub bx,[currentSegmOfFreeCOM]
       push bx
       mov [currentSegmOfFreeCOM],ax
       call SWAPXMSdirection
       mov ah,0bh
       mov si,_XMSsave
       callXMS
       call SWAPXMSdirection
       pop bx
       cmp ax,1
       jnz swaperr
       mov ax,[_mySS]
       add ax,bx
       mov ss,ax
       mov sp,[_mySP]
       ; add calling stuff here
;       push es
;       push ds
       push si
;       push word 1
;       call _process_input
       call _my2e_parsecommandline
       push ax
       mov ah,0bh       ; move into XMS again
       mov si,_XMSsave
       callXMS
       call SWAPXMSdirection
       pop ax
       jmp finish
swaperr:
       mov ax,0FFFFh
finish:
;       pop cs
;       pop ss
;       pop sp
       iret
