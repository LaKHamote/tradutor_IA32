section .bss
		LABEL12 resd 1
		LABEL7 resd 1
		LABEL1 resd 1
		LABEL3 resd 1

section .data
		LABEL19 dd 2
		LABEL16 dd 10

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
		jmp LABEL5
LABEL6:
		add DWORD [LABEL7], 30h
		mov EAX, 4
		mov EBX, 1
		mov ECX, LABEL7
		mov EDX, 4
		int 80h
		mov EAX, 1
		mov EBX, 0
		int 80h
LABEL5:
		mov EAX, [LABEL1]
		mov EBX, [LABEL3]
		imul EBX
		mov [LABEL12], EAX
		mov [LABEL7], EAX
		add DWORD [LABEL12], 30h
		mov EAX, 4
		mov EBX, 1
		mov ECX, LABEL12
		mov EDX, 4
		int 80h
		add DWORD [LABEL16], 30h
		mov EAX, 4
		mov EBX, 1
		mov ECX, LABEL16
		mov EDX, 4
		int 80h
		mov EAX, [LABEL7]
		mov EBX, [LABEL19]
		cdq
		idiv EBX
		mov EBX, [LABEL19]
		imul EBX
		mov [LABEL1], EAX
		mov EAX, [LABEL7]
		mov EBX,[LABEL1]
		sub EAX, EBX
		mov [LABEL7], EAX
		jmp LABEL6
