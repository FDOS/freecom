%include "resource.id"
%macro resIdBlock 1-2 -1
	dd	???ende - ???start		;; Length of this resource
	dw %1						;; major resource ID
	dw %2						;; minor resource ID
	db 'FREECOM '				;; cookie
%endmacro
