section .data
    buffer db 11  ; Buffer para armazenar a string de entrada (máx 10 caracteres + 1 para o null terminator)
    buffer_len equ $ - buffer
    hello db 'Hello, World!', 0xA  ; A string com a mensagem e um caractere de nova linha (0xA)
    hello_len equ $ - hello         ; Calcula o comprimento da string

section .bss
    num resd 1    ; Variável para armazenar o número lido

section .text
    global input_function

input_function:
;----------------------------escreve-label---------------------------
   ; Recebe o ponteiro para o file descriptor em [esp+4]
   ; Recebe o endereço da string em [esp+8]
   ; Recebe o comprimento da string em [esp+12]

escreve_label:
    ; Syscall para escrever no arquivo
    mov eax, 4               ; syscall number (sys_write)
    mov ebx, [esp+4]         ; file descriptor
    mov ecx, [esp+8]         ; endereço da string
    mov edx, [esp+12]        ; comprimento da string
    int 0x80                 ; invoke syscall

    ; Syscall para escrever no arquivo
    mov eax, 4               ; syscall number (sys_write)
    mov ebx, [esp+4]         ; file descriptor
    mov ecx, hello         ; endereço da string
    mov edx, hello_len        ; comprimento da string
    int 0x80                 ; invoke syscall
    ret













;;----------------------------oq-escrever----------------------------
;    ; Ler a entrada do usuário
;    mov eax, 3          ; syscall read
;    mov ebx, 0          ; stdin
;    mov ecx, buffer     ; Endereço do buffer de entrada
;    mov edx, buffer_len ; Tamanho máximo da entrada
;    int 0x80            ; Chamar a interrupção do sistema
;
;    ; Converter a string para número inteiro
;    mov esi, buffer     ; Colocar o endereço do buffer em ESI
;    call str_to_int     ; Chamar a função de conversão
;
;    ; Armazenar o número lido em `num`
;    mov [num], eax
;
;    ; Terminar o programa
;    mov eax, 1          ; syscall exit
;    xor ebx, ebx        ; Código de saída 0
;    int 0x80            ; Chamar a interrupção do sistema
;
;; Função para converter string para inteiro (str_to_int)
;str_to_int:
;    xor eax, eax        ; Zerar EAX (acumulador para o número final)
;    xor ebx, ebx        ; Zerar EBX (para verificar o sinal)
;    xor edx, edx        ; Zerar EDX (usado para multiplicações)
;
;    ; Verificar se o primeiro caractere é um sinal
;    cmp byte [esi], '-' 
;    ;jne .convert_loop  ; Se não for '-', iniciar a conversão
;    inc esi             ; Incrementar ESI para ignorar o sinal
;    mov bl, 1           ; Sinal negativo
;
;.convert_loop:
;    movzx ecx, byte [esi] ; Carregar o próximo caractere
;    cmp ecx, 0x0A         ; Verificar se é newline (fim da string)
;    je .end_convert       ; Se for, terminar a conversão
;    sub ecx, '0'          ; Converter de ASCII para valor numérico
;    imul eax, eax, 10     ; Multiplicar o número atual por 10
;    add eax, ecx          ; Adicionar o próximo dígito
;    inc esi               ; Ir para o próximo caractere
;    jmp .convert_loop     ; Repetir para o próximo caractere
;
;.end_convert:
;    cmp bl, 1             ; Verificar se o número é negativo
;    jne .done             ; Se não for, terminar
;    neg eax               ; Se for, negar o valor em EAX
;
;.done:
;    ret                   ; Retornar com o valor em EAX
;
;
;
;


; output << "\t\tmov EAX, 3\n";
; output << "\t\tmov EBX, 0\n";
; output << "\t\tmov ECX, "<< param<<"\n";
; output << "\t\tmov EDX, 1\n"; // um unico digito de int
; output << "\t\tint 80h\n";
; //subtrair 30h pra pegar um unico digito de int
; output << "\t\tsub DWORD ["<<param<<"], 30h\n";