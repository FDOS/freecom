;   $Id$
;
;	Dummy Critical Error Handler
;	Simply fails any request
;	Full relocable
;
;   $Log$
;   Revision 1.1.2.1  2000/12/17 21:57:36  skaus
;   intermediate update 1
;

	mov al, 3	; FAIL
	iret
