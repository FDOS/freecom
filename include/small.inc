
; Small header
%ifidn __OUTPUT_FORMAT__, obj
group	DGROUP		_DATA _BSS
segment	_TEXT		class=CODE
segment	_DATA		class=DATA align=2
segment	_BSS		class=BSS align=2
%else
bits 16
%define _TEXT .text
%define _DATA .data
%define _BSS .bss
%endif

@CodeSize EQU 0
@DataSize EQU 0
