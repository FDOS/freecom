;; Installable Command sample template
;;

	cmp ah, 0aeh
	jne chain
	or al, al
	je check
	cmp al, 1
	je perform
chain:
	jmp word 0:0
chain_addr:

chain2:
	popa
	jmp short chain

check:
	pusha
	mov di, sFctCheck
	call amicalled
	jc chain2
	popa
	mov al, 0ffh			;; OK, it's me
	iret

perform:
	pusha
	mov di, sFctPerform
	call amicalled
	jc chain2
	popa
	icmdAction
	iret

amicalled:
	push bx
	mov bx, cs
	mov es, bx
	cld
	mov bx, di
	call oASCIIZ
	mov bx, sCmd
	call oASCIIZ
	push es
	mov ax, ds
	mov es, ax
	mov bx, si
	call oPString
	pop es
	mov bx, sNL
	call oASCIIZ
	mov bx, sLine
	call oASCIIZ
	pop bx
	push bx
	inc bx
	push es
	mov ax, ds
	mov es, ax
	call oPString
	pop es
	mov bx, sNL
	call oASCIIZ
	pop bx
	mov di, myName
	xor ax, ax
	mov al, [SI]
	cmp al, [ES:myNameLen]
	jne notme
	mov cx, ax
	inc si
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
sByte:	DB '<'
sB		DB 0, 0, '>'
sFctPerform:	DB '>>', 1, ': perform', 13, 10, 0
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
