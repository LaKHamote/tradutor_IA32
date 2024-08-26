section .text 
	global _start 
_start:
	mov EAX, [LABEL0]
	mov EBX, [LABEL1]
	cdq
	idiv EBX
	mov [LABEL2], EAX
	mov EBX, [LABEL1]
	imull EBX
	mov [LABEL3], EAX
	mov EAX, [LABEL0]
	mov EBX,[LABEL3]
	sub EAX, EBX
	mov [LABEL3], EAX
	mov ECX, [LABEL0]
	mov [LABEL2], ECX
	mov EAX, [LABEL0]
	cmp EAX, 0
	jg LABEL4
	mov EAX, 1
	mov EBX, 0
	int 80h
section .data
	LABEL1 dd 2
section .bss
	LABEL0 resd 1
	LABEL2 resd 1
	LABEL3 resd 1
