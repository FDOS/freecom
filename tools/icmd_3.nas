;; Installable Command sample #3
;;
;;	Maps the command into ECHO.
;;

;; %define DEBUG_ME 1

%ifdef DEBUG_ME
	mov ax, cs
	add ax, 10h
	mov ds, ax
	mov es, ax
	push ax
	mov ax, start
	push ax
	mov di, myNameLen
	mov si, tstName
	xor cx, cx
	mov cl, [si]
	inc cl
	cld
	rep movsb
	mov ax, 0ae01h
	mov bx, tstLine
	mov si, tstName
	mov dx, -1
	mov es, dx
	mov cx, 5678h
	retf

tstLine: DB 128,11,'TST11 12345',13,0
	times 130-($-tstLine) DB 0
tstName: DB 5,'TST11'
	times 7 DB ' '

start:
%endif


%macro icmdAction 0
	; adjust command line buffer
	mov ax, ds
	mov es, ax
	xor ax, ax
	mov al, [si]			;; icmd's own length
	sub al, [CS:newCommandName]
	jz act0	; nothing to do
	jc act2	; command line grows
	; command line shrinks
	inc bx
	sub BYTE [bx], al
	jc error
	inc bx
	mov di, bx
	add bx, ax
	mov al, [di-2]
	mov cx, ax
	mov si, bx
	rep movsb
	jmp short act5

error:
	mov ax, cs
	mov es, ax
	mov bx, errString
	call oASCIIZ
	mov bx, [bp+6]
	mov ds, [bp+2]
	mov BYTE [bx], 0
	jmp short actEnde

act2:	;	command line grows
	mov ch, ah
	inc bx
	sub BYTE [bx], al
	jnc error
	mov di, bx
	dec ah
	add bx, ax
	mov cl, [di-1]
	cmp cl, [di]
	jc error
	add di, cx
	add bx, cx
	std
	mov si, bx
	rep movsb
	inc di
	sub cx, ax
	mov al, '.'
	rep stosb

act5:
act4:
act0:
	mov es, [bp+2]
	mov ax, cs
	mov ds, ax
	mov di, [bp+6]
	mov si, newCommandName
	xor ax, ax
	mov al, [si]			;; get new command's length of name
	inc al					;; this byte itself
	mov cx, ax
	cld
	rep movsb
actEnde:
%endmacro

%macro icmdComment 0
	DB 'Maps the command into ECHO. and outputs lots of debug information'
%endmacro

%macro icmdStuff 0
newCommandName: DB 4, 'ECHO'
errString: DB 7,'Command line buffer over- or underflow',13,10,0
%endmacro

;; include the ICMD template
%include 'icmd_inc.inc'
