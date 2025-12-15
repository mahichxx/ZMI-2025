.586
.model flat, stdcall
includelib kernel32.lib
includelib msvcrtd.lib
includelib ucrtd.lib
includelib vcruntimed.lib
includelib legacy_stdio_definitions.lib
includelib "D:\œÓ„‡ÏÏËÓ‚‡ÌËÂ\3_ÒÂÏ\ œŒ\ZMI-2025\Debug\InLib.lib"

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
	L29	db '------- 1. DATA TYPES TEST -------', 0
	L31	db '------- 1. “≈—“ “»œŒ¬ ƒ¿ÕÕ€’ -------', 0
	L34	sdword 100
	L36	db 'Decimal (100):', 0
	L39	sdword 255
	L41	db 'Hex (0xFF -> -1):', 0
	L44	sdword 5
	L46	db 'Binary (0b101):', 0
	L49	byte 'Z'
	L51	db 'Char (Z -> 90):', 0
	L54	sdword 1
	L56	db 'Bool (true -> 1):', 0
	L59	db '------- 2. MATH & FUNCTIONS -------', 0
	L61	db '------- 2.Ã¿“≈Ã¿“» ¿ & ‘”Õ ÷»» -------', 0
	L64	sdword 3
	L66	db '100 / 3 =', 0
	L71	db '100 % 3 =', 0
	L74	sdword 10
	L75	sdword 5
	L77	db 'calc(10, 5) -> (10+5) * 2 =', 0
	L81	sdword -50
	L83	db 'mabs(-50) =', 0
	L86	db '------- 3. STRINGS (Cyrillic) -------', 0
	L88	db '------- 3. —“–Œ » ( ËËÎÎËˆ‡) -------', 0
	L90	db 'œË‚ÂÚ', 0
	L92	db 'Length of Privet (expect 6):', 0
	L95	db '42', 0
	L97	db 'String 42 to Byte:', 0
	L100	db '------- 4. COMPARISON TEST -------', 0
	L102	db '------- 4. —–¿¬Õ»“≈À‹Õ€… “≈—“ -------', 0
	L106	db '10 == 10 (Expect 1):', 0
	L111	db '10 != 5 (Expect 1):', 0
	L114	sdword 20
	L117	db '20 > 10 (Expect 1):', 0
	L122	db '5 < 10 (Expect 1):', 0
	L127	db '10 <= 10 (Expect 1):', 0
	L132	db '5 >= 10 (Expect 0):', 0
	L135	db '------- 5. LOGIC & RANDOM -------', 0
	L137	db '------- 5. ÀŒ√» ¿ & —À”◊¿…ÕŒ—“‹ -------', 0
	L140	db 'Random (0 or 1):', 0
	L143	db 'Case 0: Zero/ÕÓÎ¸', 0
	L145	db 'Case 1: One/Œ‰ËÌ', 0
	L147	db 'Default/œÓ ÛÏÓÎ˜‡ÌË˛', 0
	L149	db '=== TEST COMPLETE ===', 0
	L151	db '------- “≈—“ «¿¬≈–ÿ≈Õ -------', 0
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
	push offset L31
	pop eax
	mov welcome, eax
	push welcome
	pop eax
	invoke outstr, eax
	invoke newline
	push 100
	pop eax
	mov numDec, al
	push offset L36
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
	push offset L41
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
	push offset L46
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
	push offset L51
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
	push offset L56
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, myBool
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L59
	pop eax
	invoke outstr, eax
	invoke newline
	push offset L61
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
	push offset L66
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
	push offset L71
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
	push offset L77
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
	push offset L83
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcres
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L86
	pop eax
	invoke outstr, eax
	invoke newline
	push offset L88
	pop eax
	invoke outstr, eax
	invoke newline
	push offset L90
	call strle
	push eax
	pop eax
	mov lenVal, al
	push offset L92
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, lenVal
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L95
	call strtoint
	push eax
	pop eax
	mov calcres, al
	push offset L97
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcres
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L100
	pop eax
	invoke outstr, eax
	invoke newline
	push offset L102
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
	push offset L106
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
	push offset L111
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
	push offset L117
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
	push offset L122
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
	push offset L127
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
	push offset L132
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcres
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L135
	pop eax
	invoke outstr, eax
	invoke newline
	push offset L137
	pop eax
	invoke outstr, eax
	invoke newline
	push 2
	call rnd
	push eax
	pop eax
	mov rndVal, al
	push offset L140
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
	jne sw_413_next_0
	push offset L143
	pop eax
	invoke outstr, eax
	invoke newline
	jmp sw_413_end
sw_413_next_0:
	mov eax, switch_val
	cmp eax, 1
	jne sw_413_next_1
	push offset L145
	pop eax
	invoke outstr, eax
	invoke newline
	jmp sw_413_end
sw_413_next_1:
	push offset L147
	pop eax
	invoke outstr, eax
	invoke newline
sw_413_end:
	push offset L149
	pop eax
	invoke outstr, eax
	invoke newline
	push offset L151
	pop eax
	invoke outstr, eax
	invoke newline
	invoke ExitProcess, 0
main ENDP
end main
