; $Id$
;	Criter and ^Break handler for external programs for XMS Swap
;	variant

segment _TEXT class=CODE

	global _lowlevel_cbreak_handler
_lowlevel_cbreak_handler:
%include "../criter/dmy_cbrk.asm"
