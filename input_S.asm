section .bss
    DATA resd 1
    param1 resd 5

section .data 
    ; param1 db "LUCAS", 0Ah
    param2 dd 5
    str_zero dd 48

    str_lido db "Foram lidos ", 0;
    len_lido equ $-str_lido;

    str_bytes db " bytes", 0xA;
    len_bytes equ $-str_bytes;

    str_escrito db "Foram escritos ", 0;
    len_escrito equ $-str_escrito;

    nwln db 0xA

section .text
global _start

_start:
    push param1             ;ponteiro para do que escrever
    push DWORD [param2]     ;numero de bytes pra escrever
    call s_input_function


    
    mov EAX, 1
    mov EBX, 0
    int 80h



s_input_function:

    enter 4,0 ; criar variavel para contar numero de bytes lidos
    %define num_bytes dword [EBP-4]

    push EBX
    push ECX
    push EDX

    
    mov EAX, 3
    mov EBX, 0
    mov ECX, [EBP+12]
    mov EDX, [EBP+8]
    int 80h

    mov num_bytes, 0
    mov EBX, [EBP+12] ; ponteiro pro inicio do buffer
loop:
    mov EAX, [EBP+8] ; limite de bytes lidos
    cmp num_bytes, EAX
    jz all_read
    mov EAX, [EBX] ; ler um char do buffer
    cmp EAX, 0Ah ; indice final do buffer
    jz all_read
    add num_bytes, 1
    add EBX, 1
    jmp loop
all_read:

    mov EAX, 4
    mov EBX, 1
    mov ECX, [EBP+12]
    mov EDX, num_bytes
    int 80h

    mov EAX, 4
    mov EBX, 1
    mov ECX, nwln
    mov EDX, 1
    int 80h



    mov EAX, 4
    mov EBX, 1
    add num_bytes, 48
    mov ECX, EBP
    sub ECX, 4
    mov EDX, 1
    int 80h

    pop EBX
    pop ECX
    pop EDX

    leave

    ret
