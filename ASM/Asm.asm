
.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib ../Debug/InLib.lib  ; Проверь, что имя либы совпадает с настройками проекта!

; Прототипы функций из lib.cpp
ExitProcess PROTO :DWORD
outnum      PROTO :SDWORD      ; Вывод числа
outstr      PROTO :DWORD       ; Вывод строки
newline     PROTO              ; Перевод строки

; Твои функции по варианту
strtoint    PROTO :DWORD       ; Строка -> Число
stcmp       PROTO :DWORD, :DWORD ; Сравнение строк

.stack 4096
.const
L1	db 'Resuit is', 0
L2	dd 10
L3	dd 5
L4	dd 255
L5	dd 65
L6	dd 15
L7	db 'Correct', 0
L8	dd 0
L9	db 'Zero', 0
L10	db 'Unknown', 0
L11	dd 2
L12	dd 3
switch_val dd 0
.data
		temp sdword ?
		buffer byte 256 dup(0)
res	sbyte 0
msg	dd 0
x	sbyte 0
y	sbyte 0
hexVal	sbyte 0
binVal	sbyte 0
symbol	sbyte 0
s	dd 0
result	sbyte 0
.code
int_to_char PROC uses eax ebx ecx edi esi,
pstr: dword,
intfield : sdword
mov edi, pstr
mov esi, 0
mov eax, intfield
cdq
mov ebx, 10
idiv ebx
test eax, 80000000h
jz plus
neg eax
neg edx
mov cl, '-'
mov[edi], cl
inc edi
plus :
push dx
inc esi
test eax, eax
jz fin
cdq
idiv ebx
jmp plus
fin :
mov ecx, esi
write :
pop bx
add bl, '0'
mov[edi], bl
inc edi
loop write
ret
int_to_char ENDP
mov al, a
movsx eax, al
push eax
mov al, b
movsx eax, al
push eax
mov al, res
movsx eax, al
push eax
mov al, res
movsx eax, al
push eax
pop eax
mov res, al
mov al, a
movsx eax, al
push eax
mov al, b
movsx eax, al
push eax
pop ebx
pop eax
add eax, ebx
push eax
mov al, res
movsx eax, al
push eax
push msg
push msg
pop eax
mov msg, eax
push offset L1
push msg
main PROC
mov al, x
movsx eax, al
push eax
mov al, y
movsx eax, al
push eax
mov al, hexVal
movsx eax, al
push eax
mov al, binVal
movsx eax, al
push eax
mov al, symbol
movsx eax, al
push eax
push s
mov al, result
movsx eax, al
push eax
mov al, x
movsx eax, al
push eax
pop eax
mov x, al
push L2
mov al, y
movsx eax, al
push eax
pop eax
mov y, al
push L3
mov al, hexVal
movsx eax, al
push eax
pop eax
mov hexVal, al
push L4
mov al, binVal
movsx eax, al
push eax
pop eax
mov binVal, al
push L3
mov al, symbol
movsx eax, al
push eax
pop eax
mov symbol, al
push L5
mov al, result
movsx eax, al
push eax
pop eax
mov result, al
mov al, x
movsx eax, al
push eax
mov al, y
movsx eax, al
push eax
call calculator
push eax
push s
pop eax
mov s, eax
call getmessage
push eax
pop eax
invoke outnum, eax
invoke newline
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
mov al, result
movsx eax, al
push eax
pop ebx
pop eax
cmp eax, ebx
mov eax, 0
setl al
push eax
pop eax
mov switch_val, eax
mov al, result
movsx eax, al
push eax
mov eax, switch_val
cmp eax, 15
jne skip_case_0
push L6
pop eax
invoke outnum, eax
invoke newline
push offset L7
pop ebx
pop eax
cmp eax, ebx
mov eax, 0
setl al
push eax
mov eax, switch_val
cmp eax, 0
jne skip_case_1
push L8
pop eax
invoke outnum, eax
invoke newline
push offset L9
pop ebx
pop eax
cmp eax, ebx
mov eax, 0
setl al
push eax
pop eax
invoke outnum, eax
invoke newline
push offset L10
pop ebx
pop eax
cmp eax, ebx
mov eax, 0
setl al
push eax
mov al, result
movsx eax, al
push eax
pop eax
mov result, al
mov al, result
movsx eax, al
push eax
push L11
pop ebx
pop eax
imul eax, ebx
push eax
mov al, result
movsx eax, al
push eax
pop eax
mov result, al
mov al, result
movsx eax, al
push eax
push L12
pop ebx
pop eax
cdq
idiv ebx
push eax
pop eax
invoke outnum, eax
invoke newline
mov al, result
movsx eax, al
push eax
pop ebx
pop eax
cmp eax, ebx
mov eax, 0
setl al
push eax
invoke ExitProcess, 0
main ENDP
end main
