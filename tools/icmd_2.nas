;; Installable Command sample #2
;;
;;	Do nothing --> if named as an internal command --> disables it
;;

%macro icmdAction 0
	mov ax, ds
	mov es, ax
	mov ax, cs
	mov ds, ax
	mov di, si
	mov si, newCommandName
	mov cx, newCommandNameEnd - newCommandName
	rep movsb
%endmacro

%macro icmdComment 0
	DB 'Do nothing (disabling internal command) and outputs lots of debug information'
%endmacro

%macro icmdStuff 0
newCommandName: DB 0, 'REM     '
newCommandNameEnd:
%endmacro

;; include the ICMD template
%include 'icmd_inc.inc'
