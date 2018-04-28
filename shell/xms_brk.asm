; $Id$
;	Criter and ^Break handler for external programs for XMS Swap
;	variant

%include "../include/model.inc"
%include "../include/stuff.inc"
segment _TEXT

	cglobal lowlevel_cbreak_handler
lowlevel_cbreak_handler:
%include "../criter/dmy_cbrk.asm"
