;; Installable Command sample #3
;;
;;	Maps the command into ECHO.
;;

%macro icmdAction 0
	pusha
	xor ax, ax
	mov al, [si]			;; icmd's own length byte
	add al, 2
	add bx, ax				;; icmd's command line tail
	mov al, [bx]
	cmp al, 0dh
	mov BYTE [bx], '.'		;; Make sure an empty command line doesn't
							;; issue ECHO On/Off status
	jne act1
	mov WORD [bx+1], 0dh	;; append \xd\0
act1:
	mov ax, ds
	mov es, ax
	mov ax, cs
	mov ds, ax
	mov di, si
	mov si, newCommandName
	xor ax, ax
	mov al, [si]			;; get new command's length of name
	inc al					;; this byte itself
	mov cx, ax
	rep movsb
	popa
%endmacro

%macro icmdComment 0
	DB 'Maps the command into ECHO. and outputs lots of debug information'
%endmacro

%macro icmdStuff 0
newCommandName: DB 4, 'ECHO'
%endmacro

;; include the ICMD template
%include 'icmd_inc.inc'
