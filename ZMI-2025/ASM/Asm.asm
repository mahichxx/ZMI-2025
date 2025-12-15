.586
.model flat, stdcall
includelib kernel32.lib
includelib msvcrtd.lib
includelib ucrtd.lib
includelib vcruntimed.lib
includelib legacy_stdio_definitions.lib
includelib "D:\Программирование\3_сем\КПО\ZMI-2025\Debug\InLib.lib"

ExitProcess PROTO :DWORD
outnum PROTO :SDWORD
outstr PROTO :DWORD
newline PROTO
strtoint PROTO :DWORD
stcmp PROTO :DWORD, :DWORD
strle PROTO :DWORD
mabs PROTO :SDWORD
rnd PROTO :SDWORD

.stack 4096
.const
	L2	sdword 0
	L19	sdword 2
	L30	db 'Типы данных', 0
	L32	db 'Строковый тип данных', 0
	L35	sdword 100
	L37	db 'Decimal (100):', 0
	L40	sdword 255
	L42	db 'Hex (0xFF -> -1):', 0
	L45	sdword 5
	L47	db 'Binary (0b101):', 0
	L50	byte 'Z'
	L52	db 'Char (Z -> 90):', 0
	L55	sdword 1
	L57	db 'Bool (true -> 1):', 0
	L60	db 'Math | Математика', 0
	L63	sdword 3
	L65	db '100 / 3 =', 0
	L70	db '100 % 3 =', 0
	L73	sdword 10
	L74	sdword 5
	L76	db 'calc(10, 5) -> (10+5) * 2 =', 0
	L80	sdword -50
	L82	db 'mabs(-50) =', 0
	L85	db 'Strings | Строки ', 0
	L87	db 'Привет', 0
	L89	db 'Length of Privet (expect 6):', 0
	L92	db '42', 0
	L94	db 'String 42 to Byte:', 0
	L97	db 'Comparison | Cравнение ', 0
	L101	db '10 == 10 (Expect 1):', 0
	L106	db '10 != 5 (Expect 1):', 0
	L109	sdword 20
	L112	db '20 > 10 (Expect 1):', 0
	L117	db '5 < 10 (Expect 1):', 0
	L122	db '10 <= 10 (Expect 1):', 0
	L127	db '5 >= 10 (Expect 0):', 0
	L130	db 'Conditional operator | Условный оператор', 0
	L133	db 'Random (0 - 2):', 0
	L136	db 'Case 0: Zero/Ноль', 0
	L138	db 'Case 1: One/Один', 0
	L140	db 'Default/По умолчанию', 0
	L142	db 'The end | Конец', 0
.data
	switch_val dd 0
	calcres	sbyte 0
	welcome	dd 0
	numDec	sbyte 0
	numHex	sbyte 0
	numBin	sbyte 0
	myChar	sbyte 0
	myBool	sbyte 0
	res	sbyte 0
	lenVal	sbyte 0
	rndVal	sbyte 0
.code
calc PROC, a :DWORD, b :DWORD
	; Decl: calcres
	push a
	push b
	pop ebx
	pop eax
	add eax, ebx
	push eax
	push 2
	pop ebx
	pop eax
	imul eax, ebx
	push eax
	pop eax
	mov calcres, al
	movsx eax, calcres
	push eax
	pop eax
	ret
calc ENDP
main PROC
	; Decl: welcome
	; Decl: numDec
	; Decl: numHex
	; Decl: numBin
	; Decl: myChar
	; Decl: myBool
	; Decl: res
	; Decl: lenVal
	; Decl: rndVal
	push offset L30
	pop eax
	mov welcome, eax
	push offset L32
	pop eax
	mov welcome, eax
	push welcome
	pop eax
	invoke outstr, eax
	invoke newline
	push 100
	pop eax
	mov numDec, al
	push offset L37
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, numDec
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push 255
	pop eax
	mov numHex, al
	push offset L42
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, numHex
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push 5
	pop eax
	mov numBin, al
	push offset L47
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, numBin
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push 90
	pop eax
	mov myChar, al
	push offset L52
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, myChar
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push 1
	pop eax
	mov myBool, al
	push offset L57
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, myBool
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L60
	pop eax
	invoke outstr, eax
	invoke newline
	push 100
	push 3
	pop ebx
	pop eax
	cdq
	idiv ebx
	push eax
	pop eax
	mov res, al
	push offset L65
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, res
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push 100
	push 3
	pop ebx
	pop eax
	cdq
	idiv ebx
	mov eax, edx
	push eax
	pop eax
	mov res, al
	push offset L70
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, res
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push 10
	push 5
	call calc
	push eax
	pop eax
	mov res, al
	push offset L76
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, res
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push -50
	call mabs
	push eax
	pop eax
	mov res, al
	push offset L82
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, res
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L85
	pop eax
	invoke outstr, eax
	invoke newline
	push offset L87
	call strle
	push eax
	pop eax
	mov lenVal, al
	push offset L89
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, lenVal
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L92
	call strtoint
	push eax
	pop eax
	mov res, al
	push offset L94
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, res
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L97
	pop eax
	invoke outstr, eax
	invoke newline
	push 10
	push 10
	pop ebx
	pop eax
	cmp eax, ebx
	mov eax, 0
	sete al
	push eax
	pop eax
	mov res, al
	push offset L101
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, res
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push 10
	push 5
	pop ebx
	pop eax
	cmp eax, ebx
	mov eax, 0
	setne al
	push eax
	pop eax
	mov res, al
	push offset L106
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, res
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push 20
	push 10
	pop ebx
	pop eax
	cmp eax, ebx
	mov eax, 0
	setg al
	push eax
	pop eax
	mov res, al
	push offset L112
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, res
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push 5
	push 10
	pop ebx
	pop eax
	cmp eax, ebx
	mov eax, 0
	setl al
	push eax
	pop eax
	mov res, al
	push offset L117
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, res
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push 10
	push 10
	pop ebx
	pop eax
	cmp eax, ebx
	mov eax, 0
	setle al
	push eax
	pop eax
	mov res, al
	push offset L122
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, res
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push 5
	push 10
	pop ebx
	pop eax
	cmp eax, ebx
	mov eax, 0
	setge al
	push eax
	pop eax
	mov res, al
	push offset L127
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, res
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L130
	pop eax
	invoke outstr, eax
	invoke newline
	push 3
	call rnd
	push eax
	pop eax
	mov rndVal, al
	push offset L133
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, rndVal
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	pop eax
	mov switch_val, eax
	movsx eax, rndVal
	push eax
	mov eax, switch_val
	cmp eax, 0
	jne sw_397_next_0
	push offset L136
	pop eax
	invoke outstr, eax
	invoke newline
	jmp sw_397_end
sw_397_next_0:
	mov eax, switch_val
	cmp eax, 1
	jne sw_397_next_1
	push offset L138
	pop eax
	invoke outstr, eax
	invoke newline
	jmp sw_397_end
sw_397_next_1:
	push offset L140
	pop eax
	invoke outstr, eax
	invoke newline
sw_397_end:
	push offset L142
	pop eax
	invoke outstr, eax
	invoke newline
	invoke ExitProcess, 0
main ENDP
end main
