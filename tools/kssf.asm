;; $Id$

; Kernel Swap Support Faker

; Useage: kssf freecom [{ arguments }]
;	"freecom" is the absolute path to FreeCOM -- including drive, path
;		and extenstion
;	arguments are any arguments to passed to FreeCOM

; This program fakes all interrupts and behaviour assumed by FreeCOM
; to use the kernel swap support. See DOS\K-SWAP.TXT for more details.

%include "../context.inc"

STACK_SIZE EQU 64
%define tsrend (tsrend_ - start + 100h + ((tsrend_ - start) & 1))

	ABSOLUTE 5ch	; where the temp variables do reside
execBlock:
eb_envseg	RESW 1
eb_cmdline	RESD 1
eb_fcb		RESD 2
shellCmdLine	RESB 2
context		RESW 1
ctxt_owner	RESW 1
origParent	RESW 1
cmd_buf:

SEGMENT .text

ORG 100h		; this is an executable program, but as small as possible

start:
jmp goon

%ifdef DEBUG
strExiting	DB 13, 10, 'Exiting KSSF'
strNL		DB 13, 10, '$', 0
strNotMe	DB 'The current PSP is not me!', 7, 13, 10, '$', 0
%endif

errString DB 13, 10, 'Failed to execute shell - '
	DB 'Exiting Kernel Swap Support Faker.', 7, 13, 10
	DB 'Press any key to preceed ...', 13, 10
	DB '$', 0

errExecShell:		; Failed to execute the shell
	mov dx, errString
	mov ah, 9
	int 21h
	mov ah, 7
	int 21h
	jmp exit

err2:
	jmp err2_

tsrstart:
	mov ah, 4ah		; Resize memory
	int 21h
	jc err2

mainloop:
	xor ax, ax
	mov WORD [ES:ctxt_owner], ax	; let the shell brand this KSSF
	mov bx, execBlock
	mov ah, 4bh		; load & run shell
	int 21h
	mov sp, tsrend + STACK_SIZE	; standard stack position
	mov ax, cs
	mov ds, ax
	jc errExecShell	; failed to execute the shell
	;; Run external program if one exists
	mov ax, [context]
	or ax, ax
	jz exit		; no context -> terminate kssf
	mov es, ax
	mov ax, [ES:?envSegm]
	mov WORD [eb_envseg], ax	; applies to shell & prg
	mov ax, [ES:?prg + 2]	; segment of program name
	or ax, ax
	jz execShell
	;; execute external program
	mov dx, [ES:?prg]		; offset of program name
	les bx, [es:?cmdline]	; pointer to command line
	mov WORD [eb_cmdline], bx
	mov WORD [eb_cmdline+2], es
	mov bx, cs
	mov es, bx
	mov bx, execBlock
	mov ds, ax
	mov ax, 4b00h
	int 21h					; execute external program
	jc execPrg1
	xor ax, ax				; no error
execPrg1:
	mov sp, tsrend + STACK_SIZE	; standard stack position
	mov dx, cs
	mov ds, dx
	mov es, [context]
	mov [ES:?execErr], ax	; error code of DOS-4B API
execShell:
	mov ax, shellCmdLine
	mov [eb_cmdline], ax
	mov ax, cs
	mov [eb_cmdline+2], ax
	lds dx, [ES:?shell]
	mov es, ax
	mov ax, ds				; probe if a name exists
	or ax, ax
	jne mainloop
exit:						; exit kssf
	mov ax, cs
	mov ds, ax				; ensure DS == PSP
%ifdef DEBUG
	mov dx, strExiting
	mov ah, 9
	int 21h
	mov ah, 62h
	int 21h
	mov ax, cs
	cmp ax, bx
	je dbg_1
	mov dx, strNotMe
	mov ah, 9
	int 21h
dbg_1:
%endif
	mov ax, WORD [origParent]	; restore parentPSP to original value
	mov WORD [ds:16h], ax

	mov ax, [context]		; let context be deallocated, too
	or ax, ax
	jz exit_noCtxt
	dec ax
	mov es, ax				; MCB of context
	mov WORD [ES:1], ds		; owner of context := kssf
	mov ax, [ES:10h+?envSegm]		; environment segm
	or ax, ax
	jz exit_noCtxt
	dec ax
	mov es, ax				; MCB of env segm
	mov WORD [ES:1], ds		; owner of env segm := kssf
exit_noCtxt:
	lds dx, [old_isr21]		; old INT-21
	mov ax, 2521h
	int 21h					; install old handler
	mov ax, 4c00h
	int 21h
	int 20h

isr21:						;; DOS interrupt chain
	cmp dx, 'FD'			;; magic number
	jne isr21_chain
	cmp ax, 4bfeh
	je isr21_do
	cmp ax, 4bfdh
	je isr21_do
isr21_chain:
	jmp 0:0
old_isr21 EQU $-4

isr21_do:
	push ds
	push cs
	pop ds
	push bp
	mov bp, sp		;; BP+8 == flags
	push dx
	push ax
	mov dx, bx		;; new context
	mov ah,62h		; get current PSP
	int 21h	
	mov ax, [ctxt_owner]
	or ax, ax
	jnz isr21_1
	;; first call --> brand the KSSF to this process
	mov WORD [ctxt_owner], bx
isr21_1:
	xchg bx, dx
	pop ax
	cmp al, 0feh
	mov ax, [ctxt_owner]
	je isr21_getCtxt
	;; set Ctxt
	cmp ax, dx
	jne isr21_noAcc
isr21_setCtxt:
	xchg [context], bx
	jmp short isr21_ok

isr21_getCtxt:
	mov bx, [context]
	cmp ax, dx
	jne isr21_noAcc
isr21_ok:
	and BYTE [bp+8], 0feh		; clear Carry flag
%ifdef DEBUG
	mov ax, cs					; our PSP / PID
%endif
isr21_iret:
	pop dx
	pop bp
	pop ds
	iret
isr21_noAcc:
	or BYTE [bp+8], 1			; set Carry flag
	mov ax, 5
	jmp short isr21_iret
tsrend_:

err2_:
	mov dx, errResize
	jmp errOut

goon:	;; Initialization a.s.o.
	; Decode command line argument list
	mov ax, cs		; Get PSP
	mov ds, ax

	mov ax, 3521h
	int 21h			; Get interrupt vector
	mov WORD [old_isr21], bx
	mov WORD [old_isr21+2], es
	mov dx, isr21
	mov ax, 2521h
	int 21h			; Set interrupt vector
%ifdef DEBUG
	call printLoadSegm
%endif

	mov ax, cs
	mov es, ax

	xor ax, ax
	mov di, execBlock
	mov cx, cmd_buf - execBlock
	rep stosb		; erase the execBlock
	mov BYTE [shellCmdLine+1], 0dh

	mov bx, 80h		; length byte of command line
	mov al, [bx]
	inc al
	add bx, ax		; end of line
	mov BYTE [bx], 0dh	; even if length == 126,
						; the first byte of program is unsed
	mov si, 81h		; command line
fcom1:
	lodsb
	cmp al, 0dh		; end of line
	je err1
	cmp al, ' '+1	; whitespaces
	jc fcom1

	;; Copy the command to an unused place, because between command and
	;; argument list two bytes must be inserted.
	mov dx, cmd_buf	; where the command is temporarily stored to
	mov di, dx
fcom_cpy:
	stosb
	lodsb
	cmp al, ' ' + 1
	jnc fcom_cpy
	mov BYTE [di], 0	; end of command name
	mov ax, bx			; BX == end of command line
						; SI == begin of command line + 1
	dec si				; correction for SI + 1
	sub ax, si			; length of remaining command line
	dec si				; position to the length byte
	mov BYTE [si], al	; prefixed the command line tail with the length byte
	mov WORD [eb_cmdline], si	; DWORD eb_cmdline := DS:SI
	mov WORD [eb_cmdline+2], ds

	mov ax, cs
;;	mov WORD [ds:16h], cs	; set our owner to ourselves <-> shell hack
	xchg WORD [ds:16h], ax	; set our owner to ourselves <-> shell hack
	mov WORD [origParent], ax

	;; DS:DX == command name
	;; execBlock is filled
	mov bx, (tsrend + STACK_SIZE + 15) / 16
					; := number of paragraphes
					; required for TSR and stack
	mov sp, tsrend + STACK_SIZE	; new stack size
	jmp tsrstart

err1:
	mov dx, useage
errOut:
	mov ah, 9
	int 21h
	mov ah, 7
	int 21h
	mov ax, 4c03h
	int 21h
	int 20h

useage DB 7, 'Useage: KSSF freecom [{ arguments }]', 13, 10
	DB 9, 'freecom: absolute filename of FreeCOM', 13, 10
	DB 9, 'arguments: any optionally arguments or options', 13, 10
	DB 'Spawns FreeCOM and fakes the necessary kernel functions for', 13, 10
	DB 'the Kernel Swap Support', 7, 13, 10, '$'

errResize DB 7, 'Failed to resize KSSF memory block', 13, 10, '$'

%ifdef DEBUG
printLoadSegm:
	mov dx, dbgStr1
	mov ah, 9
	int 21h
	mov ax, cs
	call printWord
	mov dl, ':'
	mov ah, 2
	int 21h
	mov ax, isr21
	call printWord
	mov dx, dbgStr2
	mov ah, 9
	int 21h
	ret

printWord:
	push ax
	mov al, ah
	call printByte
	pop ax
printByte:
	push ax
	shr al, 4
	call printNybble
	pop ax
printNybble:
	and al, 0fh
	add al, '0'
	cmp al, '0' + 10
	jc print1
	add al, 'A' - '0' - 10
print1:
	mov dl, al
	mov ah, 2
	int 21h
	ret
dbgStr1	DB 'KSSF loaded at ', '$'
dbgStr2 DB 10, 13, '$'
%endif

END
