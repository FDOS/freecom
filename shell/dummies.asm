; $Id$

; Dummy drivers to be included into FreeCOM itself

%include "../include/model.inc"
%include "../include/stuff.inc"

segment _TEXT
	cglobal dummy_criter_handler
	cglobal end_dummy_criter_handler
dummy_criter_handler:
	mov al, 3			; always fail
	iret
end_dummy_criter_handler:
