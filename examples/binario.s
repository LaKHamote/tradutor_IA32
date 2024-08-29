section .bss
		LABEL1 resd 1
		LABEL6 resd 1
		LABEL9 resd 1

section .data
		LABEL4 dd 2

section .text 
		global _start 
_start:
		mov EAX, 3
		mov EBX, 0
		mov ECX, LABEL1
		mov EDX, 1
		int 80h
		sub DWORD [LABEL1], 30h
		mov EAX, [LABEL1]
LABEL3:
		mov EBX, [LABEL4]
		cdq
		idiv EBX
		mov [LABEL6], EAX
		mov EBX, [LABEL4]
		imul EBX
		mov [LABEL9], EAX
		mov EAX, [LABEL1]
		mov EBX,[LABEL9]
		sub EAX, EBX
		mov [LABEL9], EAX
		add DWORD [LABEL9], 30h
		mov EAX, 4
		mov EBX, 1
		mov ECX, LABEL9
		mov EDX, 4
		int 80h
		mov ECX, [LABEL6]
		mov [LABEL1], ECX
		mov EAX, [LABEL1]
		cmp EAX, 0
		jg LABEL3
		mov EAX, 1
		mov EBX, 0
		int 80h
