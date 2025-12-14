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
	L29	db '=== 1. DATA TYPES TEST ===', 0
	L32	sdword 100
	L34	db 'Decimal (100):', 0
	L37	sdword 255
	L39	db 'Hex (0xFF -> -1):', 0
	L42	sdword 5
	L44	db 'Binary (0b101):', 0
	L47	byte 'Z'
	L49	db 'Char (Z -> 90):', 0
	L52	sdword 1
	L54	db 'Bool (true -> 1):', 0
	L57	db '=== 2. MATH & FUNCTIONS ===', 0
	L60	sdword 3
	L62	db '100 / 3 =', 0
	L67	db '100 % 3 =', 0
	L70	sdword 10
	L71	sdword 5
	L73	db 'calc(10, 5) -> (10+5) * 2 =', 0
	L77	sdword -50
	L79	db 'mabs(-50) =', 0
	L82	db '=== 3. STRINGS (Cyrillic) ===', 0
	L84	db 'Привет', 0
	L86	db 'Length of Privet (expect 6):', 0
	L89	db '42', 0
	L91	db 'String 42 to Byte:', 0
	L94	db '=== 4. COMPARISON TEST ===', 0
	L98	db '10 == 10 (Expect 1):', 0
	L103	db '10 != 5 (Expect 1):', 0
	L106	sdword 20
	L109	db '20 > 10 (Expect 1):', 0
	L114	db '5 < 10 (Expect 1):', 0
	L119	db '10 <= 10 (Expect 1):', 0
	L124	db '5 >= 10 (Expect 0):', 0
	L127	db '=== 5. LOGIC & RANDOM ===', 0
	L130	db 'Random switch (0 or 1):', 0
	L133	db 'Case 0: Zero generated', 0
	L135	db 'Case 1: One generated', 0
	L137	db 'Default: Should not happen', 0
	L139	db '=== TEST COMPLETE ===', 0
.data
	switch_val dd 0
	calcres	sbyte 0
	welcome	dd 0
	numDec	sbyte 0
	numHex	sbyte 0
	numBin	sbyte 0
	myChar	sbyte 0
	myBool	sbyte 0
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
	; Decl: calcres
	; Decl: lenVal
	; Decl: rndVal
	push offset L29
	pop eax
	mov welcome, eax
	push welcome
	pop eax
	invoke outstr, eax
	invoke newline
	push 100
	pop eax
	mov numDec, al
	push offset L34
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
	push offset L39
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
	push offset L44
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
	push offset L49
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
	push offset L54
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, myBool
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L57
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
	mov calcres, al
	push offset L62
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcres
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
	mov calcres, al
	push offset L67
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcres
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push 10
	push 5
	call calc
	push eax
	pop eax
	mov calcres, al
	push offset L73
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcres
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push -50
	call mabs
	push eax
	pop eax
	mov calcres, al
	push offset L79
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcres
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L82
	pop eax
	invoke outstr, eax
	invoke newline
	push offset L84
	call strle
	push eax
	pop eax
	mov lenVal, al
	push offset L86
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, lenVal
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L89
	call strtoint
	push eax
	pop eax
	mov calcres, al
	push offset L91
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcres
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L94
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
	mov calcres, al
	push offset L98
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcres
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
	mov calcres, al
	push offset L103
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcres
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
	mov calcres, al
	push offset L109
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcres
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
	mov calcres, al
	push offset L114
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcres
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
	mov calcres, al
	push offset L119
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcres
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
	mov calcres, al
	push offset L124
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcres
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L127
	pop eax
	invoke outstr, eax
	invoke newline
	push 2
	call rnd
	push eax
	pop eax
	mov rndVal, al
	push offset L130
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
	jne sw_393_next_0
	push offset L133
	pop eax
	invoke outstr, eax
	invoke newline
	jmp sw_393_end
sw_393_next_0:
	mov eax, switch_val
	cmp eax, 1
	jne sw_393_next_1
	push offset L135
	pop eax
	invoke outstr, eax
	invoke newline
	jmp sw_393_end
sw_393_next_1:
	push offset L137
	pop eax
	invoke outstr, eax
	invoke newline
sw_393_end:
	push offset L139
	pop eax
	invoke outstr, eax
	invoke newline
	invoke ExitProcess, 0
main ENDP
end main
