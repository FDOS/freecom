;; Installable Command sample template
;;
;; DS:BX :- command line buffer <max length><cur length><command line>
;; DS:SI :- command name buffer <length><command name>
;; DX == 0xFFFF :- magic number

	cmp dx, 0ffffh	;; probe magic number
	jne chain3
	pusha		;; DI -4, SI -6, BP-8, SP-10, BX-12, DX-14, CX-16, AX-18
	push ds		;; -2
	push es		;; 0
	mov bp, sp
	cmp ah, 0aeh
	jne chain
	or al, al
	je check
	cmp al, 1
	je perform
chain:
chain2:
	pop es
	pop ds
	popa
chain3:
	jmp word 0:0
chain_addr:

check:
	mov di, sFctCheck
	call amicalled
	jc chain2
	pop es
	pop ds
	popa
	dec al				;; AL := 0xFF :- OK, it's me
	iret

perform:
	mov di, sFctPerform
	call amicalled
	jc chain2
	mov ds, [bp+2]
	mov bx, [bp+12]
	mov si, [bp+6]
	icmdAction
	mov ds, [bp+2]
	mov bx, [bp+12]
	mov si, [bp+6]
	mov di, sFctReturn
	call amicalled
	pop es
	pop ds
	popa
	iret

amicalled:
	mov bx, cs
	mov es, bx
	cld
	mov bx, di
	call oASCIIZ
	mov bx, sCmd
	call oASCIIZ
	mov es, [bp+2]
	mov bx, si
	call oPStringAddr
	mov bx, cs
	mov es, bx
	mov bx, sNL
	call oASCIIZ
	mov bx, sLine
	call oASCIIZ
	mov bx, [bp+12]
	inc bx
	mov es, [bp+2]
	call oPStringAddr
	mov bx, sNL
	call oASCIIZ
	mov di, myName-1
	xor cx, cx
	mov cl, [SI]
	inc cx
	repe cmpsb
	jne notme
	mov bx, sIsMe
	call oASCIIZ
	clc
	ret
notme:
	mov bx, sIsNotMe
	call oASCIIZ
	stc
	ret

oMyName:
	push cx
	push bx
	mov cl, [ES:myNameLen]
	mov ch, 0
	mov bx, myName
	call oString
	pop bx
	pop cx
	jmp short oStr1

oPStringAddr:
	call oESBX
oPString:
	mov cl, [ES:BX]
	mov al, cl
	call oAL
	inc bx
	mov ch, 0
	jmp short oString
oASCIIZ:
	mov cx, -1
oString:
	mov al, [ES:bx]
	inc bx
	or al, al
	je oStr2
	cmp al, 1
	je oMyName
	mov ah, 0eh
	push bx
	push bp
	mov bx, 7
	int 10h
	pop bp
	pop bx
oStr1:
	loop oString
oStr2:
	ret

oAL:
	push bx
	push cx
	mov bx, sB
	call convALtoBX
	mov bx, sByte
	mov cx, 4
	push es
	mov ax, cs
	mov es, ax
	call oString
	pop es
	pop cx
	pop bx
	ret

oESBX:
	push cx
	push es
	push bx
	mov ax, es
	mov bx, sAddr
	call convAXtoBX
	pop ax
	push ax
	mov bx, sAddr2
	call convAXtoBX
	mov cx, 9
	mov ax, cs
	mov es, ax
	mov bx, sAddr
	call oString
	pop bx
	pop es
	pop cx
	ret

convAXtoBX:
	push ax
	mov al, ah
	call convALtoBX
	inc bx
	pop ax
convALtoBX:
	push ax
	shr al, 4
	call convLowALtoBX
	inc bx
	pop ax
convLowALtoBX:
	and al, 0fh
	cmp al, 10
	jc conv1
	add al, 'A' - '0' - 10
conv1:
	add al, '0'
conv3:
	mov BYTE [CS:bx], al
	ret

	icmdStuff

sFctCheck:	DB '>>', 1, ': check'
sNL:	DB 13, 10, 0
sAddr:	DB 0,0,0,0, ':'
sAddr2: DB 0,0,0,0
sByte:	DB '<'
sB		DB 0, 0, '>'
sFctPerform:	DB '>>', 1, ': perform', 13, 10, 0
sFctReturn:	DB '>>', 1, ': return', 13, 10, 0
sCmd:	DB '>>', 1, ': command name: ', 0
sLine:	DB '>>', 1, ': command line: ', 0
sIsMe:	DB '>>', 1, ': -- is me --', 13, 10, 0
sIsNotMe:	DB '>>', 1, ': -- is NOT me --', 13, 10, 0

myNameLen:	DB 0
myName:		DB 0

DW chain_addr - 4
DW myNameLen
DB 0
	icmdComment
DB 'INSTALLABLE COMMAND - ICD v1.0'
