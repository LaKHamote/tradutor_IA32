section .bss
		LABEL1 resd 1
		LABEL6 resd 1
		LABEL9 resd 1

section .data
		LABEL4 dd 2

section .text 
		global _start 
		mov EAX, [LABEL1]
LABEL3:
		mov EBX, [LABEL4]
		cdq
		idiv EBX
		mov [LABEL6], EAX
		mov EBX, [LABEL4]
		imull EBX
		mov [LABEL9], EAX
		mov EAX, [LABEL1]
		mov EBX,[LABEL9]
		sub EAX, EBX
		mov [LABEL9], EAX
		mov ECX, [LABEL1]
		mov [LABEL6], ECX
		mov EAX, [LABEL1]
		cmp EAX, 0
		jg LABEL3
		mov EAX, 1
		mov EBX, 0
		int 80h
