.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "../Debug/InLib.lib"
ExitProcess PROTO :DWORD
outnum PROTO :SDWORD
outstr PROTO :DWORD
newline PROTO

.stack 4096
.const
	L9	db 'Resuit is', 0
	L18	sdword 10
	L20	sdword 5
	L22	sdword 255
	L25	sdword 65
	L30	sdword 15
	L32	db 'Correct', 0
	L33	sdword 0
	L35	db 'Zero', 0
	L37	db 'Unknown', 0
	L40	sdword 2
	L43	sdword 3
.data
	temp sdword ?
	switch_val sdword ?
	res	sdword 0
	msg	dd 0
	x	sdword 0
	y	sdword 0
	hexVal	sdword 0
	binVal	sdword 0
	symbol	sdword 0
	s	dd 0
	result	sdword 0
.code
main PROC
	push x
	pop eax
	invoke outnum, eax
	invoke newline
	push y
	pop eax
	invoke outnum, eax
	invoke newline
	push hexVal
	pop eax
	invoke outnum, eax
	invoke newline
	push binVal
	pop eax
	invoke outnum, eax
	invoke newline
	push symbol
	pop eax
	invoke outnum, eax
	invoke newline
	push s
	pop eax
	invoke outnum, eax
	invoke newline
	push result
	pop eax
	invoke outnum, eax
	invoke newline
	push L18
	pop eax
	mov x, eax
	push L20
	pop eax
	mov y, eax
	push L22
	pop eax
	mov hexVal, eax
	push L20
	pop eax
	mov binVal, eax
	push L25
	pop eax
	mov symbol, eax
	pop eax
	invoke outnum, eax
	invoke newline
	push x
	push y
	pop eax
	mov result, eax
	pop eax
	mov s, eax
	push s
	pop ebx
	pop eax
	cmp eax, ebx
	mov eax, 0
	setl al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push result
	pop ebx
	pop eax
	cmp eax, ebx
	mov eax, 0
	setl al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	; Switch logic start (simplified)
	push result
	push L30
	push offset L32
	pop ebx
	pop eax
	cmp eax, ebx
	mov eax, 0
	setl al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push L33
	push offset L35
	pop ebx
	pop eax
	cmp eax, ebx
	mov eax, 0
	setl al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L37
	pop ebx
	pop eax
	cmp eax, ebx
	mov eax, 0
	setl al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	pop eax
	invoke outnum, eax
	invoke newline
	push result
	push L40
	pop ebx
	pop eax
	imul eax, ebx
	push eax
	pop eax
	mov result, eax
	push result
	push L43
	pop ebx
	pop eax
	cdq
	idiv ebx
	push eax
	pop eax
	mov result, eax
	push result
	pop ebx
	pop eax
	cmp eax, ebx
	mov eax, 0
	setl al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	invoke ExitProcess, 0
main ENDP
end main
