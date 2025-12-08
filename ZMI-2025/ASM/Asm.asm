.586
.model flat, stdcall
includelib msvcrtd.lib
includelib ucrtd.lib
includelib vcruntimed.lib
includelib legacy_stdio_definitions.lib
includelib kernel32.lib
includelib "D:\Программирование\3_сем\КПО\ZMI-2025\Debug\InLib.lib"

ExitProcess PROTO :DWORD
outnum PROTO :SDWORD
outstr PROTO :DWORD
newline PROTO
stcmp PROTO :DWORD, :DWORD
strtoint PROTO :DWORD

.stack 4096

.const
	L3	sbyte 15
	L7	db 'StatusOK', 0
	L9	sbyte 0
	L20	sbyte 10
	L22	sbyte 5
	L24	sbyte -20
	L26	db 'TestOneSimpleMath', 0
	L31	db 'TestTwoLeftLiteral', 0
	L33	sbyte 100
	L37	db 'TestThreeChainCalc', 0
	L44	db 'TestFourComplexParens', 0
	L48	sbyte 2
	L62	db 'TestFiveMixedLogic', 0
	L70	sbyte 3
	L73	db 'TestSixSwitchLogic', 0
	L75	db 'YisFive', 0
	L79	db 'SumIs', 0
	L82	db 'YisTen', 0
	L87	db 'TestLibCheck', 0
	L91	db '123', 0
	L94	db 'AtoiResult', 0
	L99	db 'hello', 0
	L104	db 'CompareResult', 0
.data
	switch_val sdword 0
	s	dd 0
	x	sbyte 0
	y	sbyte 0
	z	sbyte 0
	hard	sbyte 0
	negative	sbyte 0
	info	dd 0
	res	sbyte 0
	strNum	dd 0
	str1	dd 0
	str2	dd 0
	cmpRes	sbyte 0
.code
summary PROC, a :DWORD, b :DWORD
	mov al, L3
	movsx eax, al
	push eax
	pop eax
	ret
	ret
summary ENDP
getstatus PROC
	push offset L7
	pop eax
	mov s, eax
	push s
	pop eax
	ret
	ret
getstatus ENDP
main PROC
	mov al, L20
	movsx eax, al
	push eax
	pop eax
	mov x, al
	mov al, L22
	movsx eax, al
	push eax
	pop eax
	mov y, al
	mov al, L24
	movsx eax, al
	push eax
	pop eax
	mov negative, al
	push offset L26
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, x
	movsx eax, al
	push eax
	mov al, y
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop eax
	mov z, al
	mov al, z
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L31
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, L33
	movsx eax, al
	push eax
	mov al, x
	movsx eax, al
	push eax
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	pop eax
	mov z, al
	mov al, z
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L37
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, x
	movsx eax, al
	push eax
	mov al, y
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov al, L20
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov al, L22
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop eax
	mov z, al
	mov al, z
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L44
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, x
	movsx eax, al
	push eax
	mov al, y
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov al, L48
	movsx eax, al
	push eax
	pop ebx
	pop eax
	imul eax, ebx
	push eax
	pop eax
	mov hard, al
	mov al, hard
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, x
	movsx eax, al
	push eax
	mov al, y
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov al, L22
	movsx eax, al
	push eax
	mov al, L48
	movsx eax, al
	push eax
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	pop ebx
	pop eax
	cdq
	idiv ebx
	push eax
	pop eax
	mov hard, al
	mov al, hard
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, x
	movsx eax, al
	push eax
	mov al, x
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov al, y
	movsx eax, al
	push eax
	mov al, L48
	movsx eax, al
	push eax
	pop ebx
	pop eax
	imul eax, ebx
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov al, L48
	movsx eax, al
	push eax
	pop ebx
	pop eax
	cdq
	idiv ebx
	push eax
	pop eax
	mov hard, al
	mov al, hard
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L62
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, x
	movsx eax, al
	push eax
	mov al, L20
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	mov al, y
	movsx eax, al
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop eax
	mov z, al
	mov al, z
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	mov al, L48
	movsx eax, al
	push eax
	mov al, x
	movsx eax, al
	push eax
	mov al, L70
	movsx eax, al
	push eax
	pop ebx
	pop eax
	imul eax, ebx
	push eax
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop eax
	mov z, al
	mov al, z
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L73
	pop eax
	invoke outstr, eax
	invoke newline

	; --- SWITCH 231 ---
	mov al, y
	movsx eax, al
	mov switch_val, eax
	cmp eax, 5
	je switch_231_case_5
	cmp eax, 10
	je switch_231_case_10
	jmp switch_231_default
switch_231_case_5:
	push offset L75
	pop eax
	mov info, eax
	push info
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, x
	movsx eax, al
	push eax
	mov al, y
	movsx eax, al
	push eax
	call summary
	push eax
	pop eax
	mov z, al
	push offset L79
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, z
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	jmp switch_end_231
switch_231_case_10:
	push offset L82
	pop eax
	mov info, eax
	push info
	pop eax
	invoke outstr, eax
	invoke newline
	jmp switch_end_231
switch_231_default:
	call getstatus
	push eax
	pop eax
	mov info, eax
	push info
	pop eax
	invoke outstr, eax
	invoke newline
	jmp switch_end_231
switch_end_231:
	push offset L87
	pop eax
	invoke outstr, eax
	invoke newline
	push offset L91
	pop eax
	mov strNum, eax
	push strNum
	call strtoint
	push eax
	pop eax
	mov res, al
	push offset L94
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, res
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L99
	pop eax
	mov str1, eax
	push offset L99
	pop eax
	mov str2, eax
	push str1
	push str2
	call stcmp
	push eax
	pop eax
	mov cmpRes, al
	push offset L104
	pop eax
	invoke outstr, eax
	invoke newline
	mov al, cmpRes
	movsx eax, al
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	invoke ExitProcess, 0
main ENDP
end main
