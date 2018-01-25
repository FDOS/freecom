; $Id$
;	Criter and ^Break handler for external programs for XMS Swap
;	variant

%include "../include/model.inc"
segment _TEXT

	global _lowlevel_cbreak_handler
_lowlevel_cbreak_handler:
%include "../criter/dmy_cbrk.asm"
