;   $Id$
;
;	Dummy ^Break signal catcher -- module
;	Aborts any program		<-> Activated only if FreeCOM is not active
;	Full relocable
;
;	On return the caller must return to DOS the following way:
;	if(Carry)	retf			<-- Terminate
;	if(!Carry)	retf 2			<-- Proceed / Ignore ^Break
;
;   $Log$
;   Revision 1.2  2002/04/02 18:09:31  skaus
;   add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
;
;   Revision 1.1  2001/04/23 21:35:41  skaus
;   Beta 7 changes (code split)
;
;   Revision 1.1.2.1  2001/01/28 21:23:55  skaus
;   add: Kernel Swap Support
;

		stc			;; tell DOS to terminate the program
		retf		;; no IRET!
