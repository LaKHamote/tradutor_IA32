section .bss
		LABEL1 resd 1
		LABEL3 resd 1
		LABEL7 resd 1

section .data
		LABEL5 dd 1

section .text 
		global _start 
_start:
		mov EAX, 3
		mov EBX, 0
		mov ECX, LABEL1
		mov EDX, 1
		int 80h
		sub DWORD [LABEL1], 30h
		mov EAX, 3
		mov EBX, 0
		mov ECX, LABEL3
		mov EDX, 1
		int 80h
		sub DWORD [LABEL3], 30h
		mov EAX, [LABEL5]
		mov [LABEL7], EAX
		mov EAX, [LABEL3]
LABEL9:
		cmp EAX, 0
		je LABEL10
		mov EAX, [LABEL7]
		mov EBX, [LABEL1]
		imul EBX
		mov [LABEL7], EAX
		mov EAX, [LABEL3]
		mov EBX,[LABEL5]
		sub EAX, EBX
		mov [LABEL3], EAX
		jmp LABEL9
LABEL10:
		add DWORD [LABEL7], 30h
		mov EAX, 4
		mov EBX, 1
		mov ECX, LABEL7
		mov EDX, 4
		int 80h
		mov EAX, 1
		mov EBX, 0
		int 80h
