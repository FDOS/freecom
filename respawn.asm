; Module respawner
;
; Executes an external program, aquires its exit status then respawns
; FreeCOM's transient copy.
; Also checks the validity if FreeCOM may be terminated and handles
; failures during respawning FreeCOM.
;
; The last word of the image is the offset of the entry point,
; to where the FreeCOM instance just installed the Respawner
; shall jump to when it terminates.
;
; Input: nothing; current PSP == segment of context (DS:)
;	current PSP:0x80 -> maybe used as editing buffer on respawn failure
;		to fetch the new position of the FreeCOM executables
;	current's PSP environment segment -> Location to aquire the default
;		location of the FreeCOM to be respawned from

;; Enable for debugging
%define DEBUG 1
;; Enable to include English strings
%define INCLUDE_STRINGS

%ifdef DEBUG
;; Enable to compile the module into a .COM file
%define COMPILE_TO_COM

;; Enable to exclude resource ID block from file
; %define NO_RESOURCE_BLOCK
%endif

;;;;;;;;;;;;;;;; End of customization

;; Version of this module
MODULE_VERSION EQU 1

%include "resource.inc"
%include "context.inc"
%include "global.inc"

MAX_BUF EQU 128 - 3

%macro getContext 0
	call getContext_
%endmacro

%macro print 1
%endmacro

;; Used in instructions like this:	MOV AX, XBYTE 1
;; as some processors might store the constant as one byte instead of two
%define XBYTE
%define XWORD

%define execblock ?respawn_execblock + 100h
%define prgname [?respawn_prgname + 100h]
%define execblock2 ?respawn_execblockFcom + 100h
%define errorlevel [?respawn_exitCode + 100h]
%define canexit [?internal_can_exit + 100h]

;; string numbers
%define STR_notFound 0		; FreeCOM to respawn not found
%define STR_noTerminate 1	; not allowed to terminate FreeCOM
%define STR_aborted 2		; FreeCOM aborted abnormally
%define STR_execFailed 3	; exec of FreeCOM failed

%define STR_errcodes 4		; maps DOS error codes into string
							; reverse order as "errcodes" 
							; first "Unknown error"

%define NL 10, 13
%define BEEP 7

???start:

%ifdef COMPILE_TO_COM
ORG 100h
%ifndef INCLUDE_STRINGS
%define INCLUDE_STRINGS 1
%endif
%endif

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

%macro dbg_print 3+
%ifdef DEBUG
	push ds, si
	jmp %%code
%%str:
	DB %3, 0
%%code:
	call dbg_print_entry_str
	mov si, %%str
	call oStr_Buffer
	push %1
	pop ds
	mov si, %2
	call oStr_Buffer
	call oStr_Buffer_NL
	pop ds, si
%endif
%endmacro

%macro dbg_print_exec 1
%ifdef DEBUG
	push ds, si
	jmp %%code
%%str:
	DB %1, NL, 0
%%code:
	call dbg_print_entry_str
	mov si, %%str
	call dbg_print_exec_
	pop ds, si
%endif
%endmacro

%macro dbg_print_ax 1
%ifdef DEBUG
	push ds, si, f
	jmp %%code
%%str:
	DB %1, '0x'
%%strAX:	DB 'xxxx', NL, 0
%%code:
	call dbg_print_entry_str
	mov si, %%strAX
	call dbg_print_AXtoSI
	mov si, %%str
	call oStr_Buffer
	pop ds, si, f
%endif
%endmacro


execExternal:
	getContext
	mov es, bx
	lds dx, prgname
	mov bx, execblock
	dbg_print_exec 'External program'
	mov ax, 4b00h				;; Load and Execute program
	int 21h
	dbg_print_ax 'External program return code: '
	mov ah, -2					;; Indicates execute error
	jc extExecError
	;; Aquire child's exit code and reason from DOS
	mov ah, 59h
	int 21h
	dbg_print_ax 'External program exit code: '
extExecError:
	getContext
	mov WORD errorlevel, ax

	;; Going to re-spawn FreeCOM
	;; Use the contents of %COMSPEC%
execFreeCOM:
	getContext			;; when comming from error below
	;; The context is always located within the PSP, thus PSP == DS
	;; Reset the buffer, which might get used as FreeCOM filename
	;; input buffer later
	mov WORD [80h], XBYTE MAX_BUF	;; [80] := 125 (maxbuf), [81] := 0 (curbuf)
	mov ds, [2ch]			;; Get env seg
	xor ax, ax
	mov si, ax
	jmp short envLoopBeg
execExternal1:
	jmp execExternal

execNewFreeCOM:
envLoopFailed:
	;; environment variable scanned through, but nothing found
	;; prompt user
	print "Cannot find FreeCOM and/or %COMSPEC% does point nowhere"
	print "Enter the location of FreeCOM, such as: C:\COMMAND.COM"
	print "Sometimes pressing F3 will bring up older locations"
	print "To terminate this FreeCOM session just hit ENTER"
	mov al, STR_notFound
	call oString
	getContext
	mov dx, 80h					;; buffer
	mov bx, dx
	mov ah, 0ah
	int 21h
	xor ax, ax
	or al, [81h]
%ifdef DEBUG
	jne ??1
	jmp terminateSession
??1:
%else
	je terminateSession			;; empty buffer --> end this FreeCOM session
%endif
	add bx, ax					;; end of buffer
	mov BYTE [bx], ah			;; make string ASCIIZ
	jmp short execBuffer

envLoopNxt:
	inc di
	mov si, di

envLoopBeg:
	cmp al, [si]
	je envLoopFailed

envLoop:
	mov cx, comspecEnd - comspec
	mov di, comspec
	repe cmpsb
	pushf

	;; advance to next variable
	mov di, si
	dec di
envAdvanceToNext:
	inc di
	cmp al, [di]
	jne envAdvanceToNext
	popf
	jne envLoopNxt
	;; FreeCOM found --> move to into buffer and execute
	mov cx, di
	sub cx, si
	mov ch, al			;; CH := 0
	cmp cl, MAX_BUF
	jc fc_OK
	mov cl, MAX_BUF
fc_OK:
	mov di, XBYTE 82h
	mov [ES:81h], cl		;; length of string in buffer
	rep movsb

execBuffer:
	getContext
	mov es, bx
	mov bx, execblock2
	mov dx, XBYTE 82h		;; FreeCOM's name located at PSP:82h
	dbg_print_exec 'FreeCOM'
	mov ax, 4b00h
	int 21h
???entry_point:
	dbg_print_ax 'FreeCOM execution return code: '
	jc respawn_failed
	;; FreeCOM was respawned successfully and may either terminate the
	;; the session or wants to execute an external program
	mov ah, 59h					;; Get exit code
	int 21h
	cmp ax, XBYTE FREECOM_ERRLVL_SPAWN_EXTERNAL
%ifdef DEBUG
	jne ??2
	jmp execExternal1
??2:
%else
	je execExternal1
%endif
	cmp ax, XBYTE FREECOM_ERRLVL_TERMINATE
	jne freecom_aborted

terminateSession:
	getContext
	inc BYTE canexit
	jne freecom_no_exit			;; may not terminate!!
	mov ah, 4ch					;; Terminate
	mov al, errorlevel
	int 21h
	int 20h

freecom_no_exit:
	print "Not allowed to terminate FreeCOM, respawning..."
	mov al, STR_noTerminate
	call oString
	jmp execFreeCOM

freecom_aborted:
	print "Catched aborted FreeCOM, respawning..."
	mov al, STR_aborted
	call oString
	jmp execFreeCOM

respawn_failed:
	print "Execution of FreeCOM failed:"
	push ax
	mov al, STR_execFailed
	call oString
	pop ax
	mov di, errcodes
	mov cx, XBYTE errcodesEnd - errcodes
	repne scasb
	mov al, cl
	add al, STR_errcodes + 1
	call oString
	jmp execNewFreeCOM

; Get Current Context
;	Input: nothing
;	Output: BX := DS := pointer to PSP
;		destroyed: flags
getContext_:
	;; The context is located as PSP:0x100
	push ax
	mov ah, 51h				; Get Current PSP
	int 21h
	mov ds, bx
	pop ax
	ret

; Output string through BIOS
; The strings are located at the end of this module, thus, allowing
; to internationalize this module easily.
;	Input: AL == number of string to display
;	Output: destroyed: all registers except DS
;		ES := CS
;; Using BIOS instead of DOS API may impose problems if CTTY'ed.
;; In such case oString would need to open its own CON: device, because
;; the error should reach the screen, even if the current session
;; is redirected.
oString:
	push bp				; Maybe destroyed by INT-10-0E
	push ds
	mov ah, 0
%ifdef DEBUG
	cmp al, [strTable]
	jc oStr2
	mov si, strInvalid
	jmp short oStr3
oStr2:
%endif
	add ax, ax
	mov si, ax
	mov ax, cs
	mov ds, ax
	mov si, [si + strTable + 1]
oStr3:
	mov ah, 0fh			; Get Video mode
	int 10h				; BH := Active Page
	mov bl, 7			; Failsafe attribute code
	jmp short oStr_LoopBegin
oStr_Loop:
	mov ah, 0eh
	int 10h
oStr_LoopBegin:
	lodsb
	or al, al
	jne oStr_Loop
	pop ds
	pop bp
	ret

%ifdef DEBUG
?dbgEntryStr:	DB 'RESPAWNER: DEBUG: ', 0
?dbgNL	DB NL, 0
oStr_Buffer_NL:
	mov si, ?dbgNL
	jmp short oStr_Buffer_CS
dbg_print_entry_str:
	mov si, ?dbgEntryStr
oStr_Buffer_CS:
	push cs
	pop ds
oStr_Buffer:
	push ax, bx, bp, f
	mov ah, 0fh			; Get Video mode
	int 10h				; BH := Active Page
	mov bl, 7			; Failsafe attribute code
	jmp short oBuf_LoopBegin
oBuf_Loop:
	mov ah, 0eh
	int 10h
oBuf_LoopBegin:
	lodsb
	or al, al
	jne oBuf_Loop
	pop ax, bx, bp, f
	ret

dbg_print_exec_:
	push cs
	pop ds
	call oStr_Buffer
	dbg_print ds, dx, 'prg name:', NL
	lds si, [es:bx+2]
	dbg_print ds, si, 'cmd line:', NL
	ret

dbg_print_AXtoSI:
	push ax
	mov al, ah
	call dbg_print_ALtoSI
	pop ax
dbg_print_ALtoSI:
	push ax
	shr al, 4
	call dbg_print_AL2
	pop ax
dbg_print_AL2:
	push ax
	and al, 0fh
	cmp al, 10
	jc dbg_print_AL1
	add al, 'A' - '0' - 10
dbg_print_AL1:
	add al, '0'
	mov BYTE [si], al
	inc si
	pop ax
	ret
%endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; Data portion
%ifdef DEBUG
strInvalid:	DB 'RESPAWNER: Internal ERROR: Invalid string ID!', BEEP, NL, 0
%endif

errcodes:	DB 02h,05h,08h,0Ah,0Bh
errcodesEnd:

comspec:	DB 'COMSPEC='	; Variable containing the name of FreeCOM executable
comspecEnd:

strTable:
%ifdef INCLUDE_STRINGS
	DB 10			;; number of strings
	DW str0		; not found
	DW str1		; cannot terminate
	DW str2		; aborted
	DW str3		; respawn failed

	DW errUnknown
	DW err0B
	DW err0A
	DW err08
	DW err05
	DW err02

str0: 
	DB NL, 'Cannot find FreeCOM and/or %COMSPEC% does point nowhere', NL
	DB 'Enter the location of FreeCOM, such as: C:\COMMAND.COM', NL
	DB 'Sometimes pressing F3 will bring up an elder location', NL
	DB 'To terminate this FreeCOM session just hit ENTER', NL, BEEP, 0

str1:
	DB NL, 'Not allowed to terminate FreeCOM, respawning...', NL, BEEP, 0

str2:
	DB NL, 'FreeCOM aborted abnormally, respawning...', NL, BEEP, 0

str3:
	DB NL, 'Execution of FreeCOM failed: ', BEEP, 0


errUnknown:		DB "Unknown error", NL, 0
err0B:			DB "Invalid format", NL, 0
err0A:			DB "Invalid environment", NL, 0
err08:			DB "Insufficient memory", NL, 0
err05:			DB "Access denied", NL, 0
err02:			DB "File not found", NL, 0
%endif

DW ???entry_point

???ende:

%ifndef NO_RESOURCE_BLOCK
;; Include resource block
%ifndef INCLUDE_STRINGS
	resIdBlock RES_ID_RESPAWNER, 1
%else
	resIdBlock RES_ID_RESPAWNER, 3
%endif
%endif

END

