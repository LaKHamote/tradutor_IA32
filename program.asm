section .bss
		LABEL8 resd 1
		LABEL1 resd 1

section .data
		LABEL4 dd 1

section .text 
		global _start 
_start:
		mov EAX, 3
		mov EBX, 0
		mov ECX, LABEL1
		mov EDX, 4
		int 80h
		mov EAX, [LABEL1]
LABEL3:
		mov EBX,[LABEL4]
		sub EAX, EBX
		cmp EAX, 0
		je LABEL6
		mov [LABEL8], EAX
		mov EBX, [LABEL1]
		imul EBX
		mov [LABEL1], EAX
		mov EAX, [LABEL8]
		jmp LABEL3
LABEL6:
		mov EAX, 4
		mov EBX, 1
		mov ECX, LABEL1
		mov EDX, 4
		int 80h
		mov EAX, 1
		mov EBX, 0
		int 80h
