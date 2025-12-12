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
	L26	db '=== START FINAL TEST ===', 0
	L29	db 'Проверка кириллицы: Успешно!', 0
	L32	db '--- Testing Libraries ---', 0
	L35	db 'Length of welcome string (expect 24):', 0
	L39	sdword -120
	L42	db 'Abs(-120) result:', 0
	L45	sdword 10
	L47	db 'Random value (0-9):', 0
	L50	db '--- Math Test ---', 0
	L52	sdword 50
	L54	db '50 + 50 = ', 0
	L57	sdword 100
	L58	sdword 30
	L60	db '100 + 30 (Overflow check):', 0
	L63	db '--- Logic Test (Switch) ---', 0
	L65	sdword 1
	L67	db 'Branch 0 (Skipped)', 0
	L69	db 'Branch 1 (Selected)', 0
	L71	db 'Jackpot! Random is 0', 0
	L73	db 'Random is not 0', 0
	L75	db 'Default Branch', 0
	L77	db '=== TEST COMPLETE ===', 0
.data
	switch_val dd 0
	addersum	sbyte 0
	welcome	dd 0
	russianPhrase	dd 0
	number	sbyte 0
	lenVal	sbyte 0
	randomVal	sbyte 0
	selector	sbyte 0
	calcResult	sbyte 0
.code
adder PROC, a :DWORD, b :DWORD
	push a
	push b
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop eax
	mov addersum, al
	pop eax
	ret
	movsx eax, addersum
	push eax
adder ENDP
main PROC
	push offset L26
	pop eax
	mov welcome, eax
	push welcome
	pop eax
	invoke outstr, eax
	invoke newline
	push offset L29
	pop eax
	mov russianPhrase, eax
	push russianPhrase
	pop eax
	invoke outstr, eax
	invoke newline
	push offset L32
	pop eax
	invoke outstr, eax
	invoke newline
	push welcome
	call strle
	push eax
	pop eax
	mov lenVal, al
	push offset L35
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, lenVal
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push -120
	pop eax
	mov number, al
	movsx eax, number
	push eax
	call mabs
	push eax
	pop eax
	mov number, al
	push offset L42
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, number
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push 10
	call rnd
	push eax
	pop eax
	mov randomVal, al
	push offset L47
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, randomVal
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L50
	pop eax
	invoke outstr, eax
	invoke newline
	push 50
	push 50
	call adder
	push eax
	pop eax
	mov calcResult, al
	push offset L54
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcResult
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push 100
	push 30
	call adder
	push eax
	pop eax
	mov calcResult, al
	push offset L60
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, calcResult
	push eax
	pop eax
	invoke outnum, eax
	invoke newline
	push offset L63
	pop eax
	invoke outstr, eax
	invoke newline
	push 1
	pop eax
	mov selector, al
	movsx eax, selector
	push eax
	pop eax
	mov switch_val, eax
	mov eax, switch_val
	cmp eax, 0
	jne sw_225_next_0
	push offset L67
	pop eax
	invoke outstr, eax
	invoke newline
	jmp sw_225_end
sw_225_next_0:
	mov eax, switch_val
	cmp eax, 1
	jne sw_225_next_1
	push offset L69
	pop eax
	invoke outstr, eax
	invoke newline
	movsx eax, randomVal
	push eax
	pop eax
	mov switch_val, eax
	mov eax, switch_val
	cmp eax, 0
	jne sw_247_next_0
	push offset L71
	pop eax
	invoke outstr, eax
	invoke newline
	jmp sw_247_end
sw_247_next_0:
	push offset L73
	pop eax
	invoke outstr, eax
	invoke newline
sw_247_end:
	jmp sw_225_end
sw_225_next_1:
	push offset L75
	pop eax
	invoke outstr, eax
	invoke newline
sw_225_end:
	push offset L77
	pop eax
	invoke outstr, eax
	invoke newline
	invoke ExitProcess, 0
main ENDP
end main
