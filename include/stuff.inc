;; Multi-Push/Pop
;; Both take the registers in the same order, "F"==Flags
;; Push pushes left to right; pop right to left

%macro pushm 2-*
%rep %0
%ifidni %1,f
	pushf
%else
	push %1 
%endif
%rotate 1 
%endrep 
%endmacro
%macro popm 2-*
%rep %0
%rotate -1 
%ifidni %1,f
	popf
%else
	pop %1 
%endif
%endrep 
%endmacro

;; declare global and extern with underscore for OMF, without for ELF

%macro  cglobal 1-*
%rep %0
%ifnidn __OUTPUT_FORMAT__,elf
	%define %1 _%1 
%endif
	global	%1
%rotate 1 
%endrep 
%endmacro

%macro  cextern 1-*
%rep %0
%ifnidn __OUTPUT_FORMAT__,elf
	%define %1 _%1 
%endif
	extern	%1
%rotate 1 
%endrep 
%endmacro
