;; Multi-Push/Pop
;; Both take the registers in the same order, "F"==Flags
;; Push pushes left to right; pop right to left

%macro push 2-*
%rep %0
%ifidni %1,f
	pushf
%else
	push %1 
%endif
%rotate 1 
%endrep 
%endmacro
%macro pop 2-*
%rep %0
%rotate -1 
%ifidni %1,f
	popf
%else
	pop %1 
%endif
%endrep 
%endmacro
