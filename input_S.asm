section .bss
    DATA resd 1
    param1 resd 5

section .data 
    output1 db "LU"
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
    push param1             ;ponteiro para onde quardar
    push DWORD [param2]     ;numero de bytes pra quardar
    call s_input_function

    push output1            ;ponteiro para do que escrever
    push DWORD [param2]     ;numero de bytes pra escrever
    call s_output_function


    
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

    mov eax, 4
    mov ebx, 1
    mov ecx, str_lido
    mov edx, len_lido
    int 0x80
    push num_bytes
    call output_function
    mov EAX, 4
    mov EBX, 1
    mov ECX, str_bytes
    mov EDX, len_bytes
    int 80h

    pop EDX
    pop ECX
    pop EBX

    leave

    ret

s_output_function:
    enter 0,0

    push EBX
    push ECX
    push EDX

    mov EAX, 4
    mov EBX, 1
    mov ECX, [EBP+12]
    mov EDX, [EBP+8]
    int 80h
    mov EAX, 4
    mov EBX, 1
    mov ECX, nwln
    mov EDX, 1
    int 80h

    mov EAX, 4
    mov EBX, 1
    mov ECX, str_escrito
    mov EDX, len_escrito
    int 0x80
    push DWORD [EBP+8]
    call output_function
    mov EAX, 4
    mov EBX, 1
    mov ECX, str_bytes
    mov EDX, len_bytes
    int 80h

    pop EDX
    pop ECX
    pop EBX

    leave

    ret




output_function:
    ; Lê o numero passado em [ebp+8] e imprime;
        enter 0,0             ; Entra no contexto de pilha em que [ebp+8] eh o argumento passado;
        mov ebx, [ebp+8]      ; Pega o argumento passado por PILHA e poem em ebx;
        xor ecx, ecx          ; Zera ECX, usado para contar o número de dígitos (inicialmente 0);
        cmp ebx, 0            ; Compara o valor de EBX com 0 para verificar se é negativo;
        jge n_positivo        ; Se EBX for maior ou igual a 0 (não negativo), salta para o rótulo n_positivo;
        mov eax, '-'          ; Se o número for negativo, coloca o caractere '-' em EAX;
        call escreve_char     ; Chama a função escreve_char para imprimir o caractere '-';
        neg ebx               ; Torna EBX positivo (se estava negativo);
    n_positivo:;
        mov eax, ebx          ; Move o valor absoluto de EBX para EAX (preparando para conversão de dígitos);
    convert_loop:;
        xor edx, edx          ; Zera EDX, necessário para a operação de divisão;
        mov ebx, 10           ; Carrega o divisor 10 em EBX para a divisão;
        div ebx               ; Divide EAX por EBX (10), resultado em EAX (quociente) e EDX (resto);
        add ecx, 1            ; Incrementa o contador de dígitos (ECX) em 1;
        add edx, '0'          ; Converte o resto (dígito) em seu valor ASCII;
        push edx              ; Empilha o caractere ASCII na pilha;
        cmp eax, 0            ; Compara o quociente (EAX) com 0;
        jne convert_loop      ; Se o quociente não for 0, continua o loop para processar o próximo dígito;
        mov ebx, ecx          ; Guarda em ebx os bytes escritos;
    imprime_pilha:;
        cmp ecx, 0            ; Compara o contador de dígitos (ECX) com 0;
        je end                ; Se ECX for 0 (não há mais dígitos para imprimir), salta para o rótulo end;
        pop eax               ; Remove o próximo caractere da pilha e coloca em EAX;
        call escreve_char     ; Chama a função escreve_char para imprimir o caractere;
        sub ecx, 1            ; Decrementa o contador de dígitos (ECX) em 1;
        add eax, 1            ; incrementa o contador de bytes (EAX) em 1;
        jmp imprime_pilha     ; Volta para o início do rótulo imprime_pilha para imprimir o próximo caractere;
    escreve_char:;
        push ecx;
        push ebx;
        push edx;
        push eax;
        mov eax, 4            ; Define o número da syscall para sys_write (4);
        mov ebx, 1            ; Define o descritor de arquivo para stdout (1);
        mov ecx, esp          ; Define o ponteiro de dados (ECX) como o topo da pilha (contém o caractere a ser escrito);
        mov edx, 1            ; Define o número de bytes a serem escritos (1 byte);
        int 0x80              ; Chama o kernel para executar a syscall sys_write;
        pop eax;
        pop edx;
        pop ebx;
        pop ecx;
        ret;
    end:;          
        mov eax,ebx           ; Guarda os bytes escritos + endline ;
        leave                 ; Sai do contexto de pilha criado para receber o argumento ;
        ret 4                 ; Retorna da função output_function;