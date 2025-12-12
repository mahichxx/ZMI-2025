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

.stack 4096
.const
	L2	sdword 0
	L11	sdword 1
	L30	db '=== STORAGE LANGUAGE TEST: START ===', 0
	L32	db 'New Syntax Check: [ OK ]', 0
	L34	sdword 10
	L36	sdword 2
	L38	db 'Test 1: Simple Overflow', 0
	L40	sdword 100
	L41	sdword 28
	L43	db '100 + 28 = 128 -> Expect -128:', 0
	L46	db 'Test 2: Multiplication', 0
	L48	sdword 5
	L50	db '(10 + 5) * 10 = 150 -> Expect -106:', 0
	L53	db 'Test 3: Recursion (2^7)', 0
	L55	sdword 7
	L57	db '2^7 = 128 -> Expect -128:', 0
	L60	db 'Test 4: Nested Check (Switch)', 0
	L63	db 'Error: Case 0', 0
	L65	db 'Layer 1: OK', 0
	L67	db 'Layer 2: OK (Val is 10)', 0
	L69	sdword 6
	L71	db '2^6 = 64 (Safe):', 0
	L74	db 'Error: Layer 2 Default', 0
	L76	db 'Error: Layer 1 Default', 0
	L78	db '=== STORAGE LANGUAGE TEST: END ===', 0
.data
	switch_val dd 0
	powerres	sbyte 0
	powernextExp	sbyte 0
	mixerres	sbyte 0
	status	dd 0
	val	sbyte 0
	key	sbyte 0
	result	sbyte 0
	flag	sbyte 0
.code
power PROC, base :DWORD, exp :DWORD
	movsx eax, powerres
	push eax
	movsx eax, powernextExp
	push eax
	pop eax
	mov switch_val, eax
	push exp
	mov eax, switch_val
	cmp eax, 0
	jne sw_44_next_0
	pop eax
	ret
	push 1
	jmp sw_44_end
sw_44_next_0:
	movsx eax, powernextExp
	push eax
	pop eax
	mov powernextExp, al
	push exp
	push 1
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	movsx eax, powerres
	push eax
	pop eax
	mov powerres, al
	push base
	push base
	movsx eax, powernextExp
	push eax
	pop ebx
	pop eax
	imul eax, ebx
	push eax
	pop eax
	ret
	movsx eax, powerres
	push eax
sw_44_end:
	pop eax
	ret
	push 0
power ENDP
mixer PROC, a :DWORD, b :DWORD, cc :DWORD
	movsx eax, mixerres
	push eax
	movsx eax, mixerres
	push eax
	pop eax
	mov mixerres, al
	push a
	push b
	pop ebx
	pop eax
	add eax, ebx
	push eax
	push cc
	pop ebx
	pop eax
	imul eax, ebx
	push eax
	pop eax
	ret
	movsx eax, mixerres
	push eax
mixer ENDP
main PROC
	push status
	movsx eax, val
	push eax
	movsx eax, key
	push eax
	movsx eax, result
	push eax
	movsx eax, flag
	push eax
	push offset L30
	pop eax
	invoke outstr, eax
	invoke newline
	push offset L32
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, val
	push eax
	pop eax
	mov val, al
	push 10
	movsx eax, key
	push eax
	pop eax
	mov key, al
	push 2
	push offset L38
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, result
	push eax
	pop eax
	mov result, al
	push 100
	push 28
	push 1
	push offset L43
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, result
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L46
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, result
	push eax
	pop eax
	mov result, al
	push 10
	push 5
	push 10
	push offset L50
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, result
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L53
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, result
	push eax
	pop eax
	mov result, al
	push 2
	push 7
	push offset L57
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, result
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L60
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, flag
	push eax
	pop eax
	mov flag, al
	push 1
	pop eax
	mov switch_val, eax
	movsx eax, flag
	push eax
	mov eax, switch_val
	cmp eax, 0
	jne sw_225_next_0
	push offset L63
	pop eax
	invoke outstr, eax
	invoke newline
	jmp sw_225_end
sw_225_next_0:
	mov eax, switch_val
	cmp eax, 1
	jne sw_225_next_1
	push offset L65
	pop eax
	invoke outstr, eax
	invoke newline
	pop eax
	mov switch_val, eax
	movsx eax, val
	push eax
	mov eax, switch_val
	cmp eax, 10
	jne sw_247_next_10
	push offset L67
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, result
	push eax
	pop eax
	mov result, al
	push 2
	push 6
	push offset L71
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, result
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	jmp sw_247_end
sw_247_next_10:
	push offset L74
	pop eax
	invoke outstr, eax
	invoke newline
sw_247_end:
	jmp sw_225_end
sw_225_next_1:
	push offset L76
	pop eax
	invoke outstr, eax
	invoke newline
sw_225_end:
	push offset L78
	pop eax
	invoke outstr, eax
	invoke newline
	invoke ExitProcess, 0
main ENDP
end main
