
; Large header
group	DGROUP		_DATA _BSS
segment	_TEXT		class=CODE
segment	_DATA		class=DATA align=2
segment	_BSS		class=BSS align=2

@CodeSize EQU 1
@DataSize EQU 1

%macro ret 0-1
	retf %1
%endmacro
