.586
.model flat, stdcall
includelib msvcrtd.lib
includelib ucrtd.lib
includelib vcruntimed.lib
includelib legacy_stdio_definitions.lib
includelib "D:\Программирование\3_сем\КПО\ZMI-2025\Debug\InLib.lib"
includelib kernel32.lib
includelib InLib.lib

ExitProcess PROTO :DWORD
outnum PROTO :SDWORD
outstr PROTO :DWORD
newline PROTO
stcmp PROTO :DWORD, :DWORD
strtoint PROTO :DWORD

.stack 4096

.const
	L2	sbyte 0
	L12	sbyte 1
	L20	db '--- Logic Comparison in Switch ---', 0
	L23	db 'Result: v1 GREATER than v2', 0
	L26	db 'Result: v1 EQUAL v2', 0
	L28	db 'Result: v1 LESS than v2', 0
	L36	db '=== ULTRA HARDCORE TEST START ===', 0
	L38	db '1. Literals and Types', 0
	L40	sbyte 127
	L42	db 'Hex 0x7F (Max 1-byte):', 0
	L45	sbyte 3
	L47	db 'Bin 0b11 (3):', 0
	L50	sbyte 65
	L52	db 'Char Literal A (65):', 0
	L55	db '2. One Byte Overflow', 0
	L59	db '127 + 1 = -128 (Overflow Check):', 0
	L62	db '3. String Library', 0
	L64	db '100', 0
	L67	db 'StrToInd 100 + 25:', 0
	L70	sbyte 25
	L72	db 'pass', 0
	L75	db 'Strcmp pass vs pass (0 is equal):', 0
	L78	db '4. Comparisons via Switch', 0
	L79	sbyte 10
	L80	sbyte 5
	L82	db '5. Recursion Sum(5) = 5+4+3+2+1', 0
	L85	db 'Expect 15:', 0
	L88	db '=== ULTRA HARDCORE TEST END ===', 0
.data
	switch_val sdword 0
	res	sbyte 0
	next	sbyte 0
	diff	sbyte 0
	hexVal	sbyte 0
	binVal	sbyte 0
	intVal	sbyte 0
	strVal	dd 0
	charVal	sbyte 0
	logicRes	sbyte 0
.code
recCheck PROC, n :DWORD

	; --- SWITCH 41 ---
	mov eax, n
	mov switch_val, eax
	cmp eax, 0
	je switch_41_case_0
	jmp switch_41_default
switch_41_case_0:
	mov al, L2
	movsx eax, al
	push eax
	pop eax
	ret
	jmp switch_end_41
switch_41_default:
	mov eax, n
	push eax
	mov al, L12
	movsx eax, al
	push eax
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	pop eax
	mov next, al
	mov eax, n
	push eax
	mov al, next
	movsx eax, al
	push eax
	call recCheck
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop eax
	mov res, al
	mov al, res
	movsx eax, al
	push eax
	pop eax
	ret
	jmp switch_end_41
switch_end_41:
	mov al, L2
	movsx eax, al
	push eax
	pop eax
	ret
	ret
recCheck ENDP
logTest PROC, v1 :DWORD, v2 :DWORD
	push offset L20
	pop eax
	invoke outstr, eax
	invoke newline

	; --- SWITCH 99 ---
	mov eax, v1
	mov switch_val, eax
	cmp eax, 1
	je switch_99_case_1
	cmp eax, 0
	je switch_99_case_0
	jmp switch_99_default
switch_99_case_1:
	push offset L23
	pop eax
	invoke outstr, eax
	invoke newline
	jmp switch_end_99
switch_99_case_0:

	; --- SWITCH 119 ---
	mov eax, v1
	mov switch_val, eax
	cmp eax, 1
	je switch_119_case_1
	jmp switch_119_default
switch_119_case_1:
	push offset L26
	pop eax
	invoke outstr, eax
	invoke newline
	jmp switch_end_119
switch_119_default:
	push offset L28
	pop eax
	invoke outstr, eax
	invoke newline
	jmp switch_end_119
switch_end_119:
	jmp switch_end_99
switch_99_default:
	jmp switch_end_99
switch_end_99:
	mov al, L2
	movsx eax, al
	push eax
	pop eax
	ret
	ret
logTest ENDP
main PROC
	mov al, charVal
	movsx eax, al
	push eax
	push offset L36
	pop eax
	invoke outstr, eax
	invoke newline
	push offset L38
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, L40
	movsx eax, al
	push eax
	pop eax
	mov hexVal, al
	push offset L42
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, hexVal
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, L45
	movsx eax, al
	push eax
	pop eax
	mov binVal, al
	push offset L47
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, binVal
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, L50
	movsx eax, al
	push eax
	pop eax
	mov charVal, al
	push offset L52
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, charVal
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L55
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, hexVal
	movsx eax, al
	push eax
	mov al, L12
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop eax
	mov intVal, al
	push offset L59
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, intVal
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L62
	pop eax
	invoke outstr, eax
	invoke newline
	push offset L64
	pop eax
	mov strVal, eax
	push strVal
	call strtoint
	push eax
	pop eax
	mov intVal, al
	push offset L67
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, intVal
	movsx eax, al
	push eax
	mov al, L70
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L72
	pop eax
	mov strVal, eax
	push offset L72
	push strVal
	call stcmp
	push eax
	pop eax
	mov logicRes, al
	push offset L75
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, logicRes
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L78
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, L80
	movsx eax, al
	push eax
	mov al, L79
	movsx eax, al
	push eax
	call logTest
	push eax
	mov al, L79
	movsx eax, al
	push eax
	mov al, L80
	movsx eax, al
	push eax
	call logTest
	push eax
	mov al, L80
	movsx eax, al
	push eax
	mov al, L80
	movsx eax, al
	push eax
	call logTest
	push eax
	push offset L82
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, L80
	movsx eax, al
	push eax
	call recCheck
	push eax
	pop eax
	mov intVal, al
	push offset L85
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, intVal
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L88
	pop eax
	invoke outstr, eax
	invoke newline
	invoke ExitProcess, 0
main ENDP
end main
