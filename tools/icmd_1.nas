;; Installable Command sample #1
;;
;;	Echo command line arguments to screen via INT-10-0E
;;

%macro icmdAction 0
	mov ax, ds
	mov es, ax
	xor ax, ax
	mov BYTE [si], al		;; zero the length byte --> shell stops interprete
	inc bx
	mov al, [bx]
	mov cx, ax
	inc bx
	call oString
%endmacro

%macro icmdComment 0
	DB 'Displays command line and outputs lots of debug information'
%endmacro

%macro icmdStuff 0
%endmacro

;; include the ICMD template
%include 'icmd_inc.inc'
