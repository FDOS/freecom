; Display the command line
; THE LAST CHARACTER MUST BE A DOLLAR SIGN!!!
; e.g.:
; C> disp_cmd=abcd$

	mov ah, 9
	mov dx, 81h
	int 21h
	mov ax, 4c00h
	int 21h
	int 20h

