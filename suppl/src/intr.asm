; File:
;                         intr.asm
; Description:
;       Assembly implementation of calling an interrupt
;
;                    Copyright (c) 2000
;                       Steffen Kaiser
;                       All Rights Reserved
;
; This file is part of FreeDOS.
;
; FreeDOS is free software; you can redistribute it and/or
; modify it under the terms of the GNU General Public License
; as published by the Free Software Foundation; either version
; 2, or (at your option) any later version.
;
; DOS-C is distributed in the hope that it will be useful, but
; WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
; the GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public
; License along with DOS-C; see the file COPYING.  If not,
; write to the Free Software Foundation, Inc.,
; 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
;

%ifidn __OUTPUT_FORMAT__, elf 	; only for ia16-elf-gcc compilations

segment .text

bits 16

global intr

intr:		push	bp			; Standard C entry
		mov	bp,sp
		push	si
		push	di
		push	es
		push	ds
		mov	ax, [bp+4]		; interrupt number
		mov	[cs:intr_1-1], al
		jmp	short intr_2		; flush the instruction cache
intr_2:		mov	bx, [bp+6]		; regpack structure
		mov	ax, [bx]
		mov	cx, [bx+4]
		mov	dx, [bx+6]
		mov	bp, [bx+8]
		mov	si, [bx+10]
		mov	di, [bx+12]
		push	word [bx+14]		; ds
		mov	es, [bx+16]
		mov	bx, [bx+2]
		pop	ds
		int	0
intr_1:
		pushf
		push	ds
		push	bx
		mov	bx, sp
		mov	ds, [ss:bx+6]
		mov	bx, [ss:bx+20]		; address of REGPACK
		mov	[bx], ax
		pop	word [bx+2]
		mov	[bx+4], cx
		mov	[bx+6], dx
		mov	[bx+8], bp
		mov	[bx+10], si
		mov	[bx+12], di
		pop	word [bx+14]
		mov	[bx+16], es
		pop	word [bx+18]

		pop	ds
		pop	es
		pop	di
		pop	si
		pop	bp
		ret	4
%endif
