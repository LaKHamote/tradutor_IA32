section .bss
		LABEL1 resd 1
		LABEL6 resd 1
		LABEL8 resd 1

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
		mov [LABEL8], EAX
		mov EAX, [LABEL1]
		mov EBX,[LABEL8]
		sub EAX, EBX
		mov [LABEL8], EAX
		add DWORD [LABEL8], 30h
		mov EAX, 4
		mov EBX, 1
		mov ECX, LABEL8
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
