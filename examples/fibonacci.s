section .bss
		LABEL2 resd 1
		LABEL7 resd 1
		LABEL13 resd 1
		LABEL9 resd 1

section .data
		LABEL1 dd 1

section .text 
		global _start 
_start:
		mov ECX, [LABEL1]
		mov [LABEL2], ECX
		mov EAX, [LABEL2]
		mov EBX,[LABEL1]
		sub EAX, EBX
		mov [LABEL2], EAX
		mov ECX, [LABEL1]
		mov [LABEL7], ECX
		mov EAX, 3
		mov EBX, 0
		mov ECX, LABEL9
		mov EDX, 1
		int 80h
		sub DWORD [LABEL9], 30h
LABEL10:
		mov EAX, [LABEL2]
		mov EBX,[LABEL7]
		add EAX, EBX
		mov [LABEL13], EAX
		mov EBX,[LABEL9]
		sub EAX, EBX
		cmp EAX, 0
		jg LABEL16
		mov ECX, [LABEL7]
		mov [LABEL2], ECX
		mov ECX, [LABEL13]
		mov [LABEL7], ECX
		jmp LABEL10
LABEL16:
		add DWORD [LABEL13], 30h
		mov EAX, 4
		mov EBX, 1
		mov ECX, LABEL13
		mov EDX, 4
		int 80h
		mov EAX, 1
		mov EBX, 0
		int 80h
