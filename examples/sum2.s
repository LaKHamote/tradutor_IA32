section .bss
		LABEL1 resd 1
		LABEL3 resd 1
		LABEL7 resd 1

section .data
		str_lido db "Bytes lidos: ", 0x0A 
		len_lido equ $-str_lido 
		str_escrito db "Bytes escritos: ", 0X0A 
		len_escrito equ $-str_escrito 

section .text 
		global _start 
_start:
		call input_function ; Input será carregado em EDX, numero de bytes lidos em EBX e EAX
		mov [LABEL1], EDX
		push EBX ; joga valor atual de EBX na stack
		push ECX ; joga valor atual de ECX na stack
		push EDX ; joga valor atual de EDX na stack
		push EAX ; joga valor atual de EAX na stack
		mov eax, 4
		mov ebx, 1
		mov ecx, str_lido
		mov edx, len_lido
		int 0x80
		pop EAX ; volta valor antigo de EAX
		pop EDX ; volta valor antigo de EDX
		pop ECX ; volta valor antigo de ECX
		pop EBX ; volta valor antigo de EBX
		call output_function ; Mostra bytes lidos no console
		call input_function ; Input será carregado em EDX, numero de bytes lidos em EBX e EAX
		mov [LABEL3], EDX
		push EBX ; joga valor atual de EBX na stack
		push ECX ; joga valor atual de ECX na stack
		push EDX ; joga valor atual de EDX na stack
		push EAX ; joga valor atual de EAX na stack
		mov eax, 4
		mov ebx, 1
		mov ecx, str_lido
		mov edx, len_lido
		int 0x80
		pop EAX ; volta valor antigo de EAX
		pop EDX ; volta valor antigo de EDX
		pop ECX ; volta valor antigo de ECX
		pop EBX ; volta valor antigo de EBX
		call output_function ; Mostra bytes lidos no console
		mov EAX, [LABEL1]
		mov EBX,[LABEL3]
		add EAX, EBX
		mov [LABEL7], EAX
		push EBX ; joga valor atual de EBX na stack
		push EAX ; joga valor atual de EAX na stack
		mov EBX, [LABEL7]
		call output_function ; Mostra label que foi colocado em EBX
		push EBX ; joga valor atual de EBX na stack
		push ECX ; joga valor atual de ECX na stack
		push EDX ; joga valor atual de EDX na stack
		push EAX ; joga valor atual de EAX na stack
		mov eax, 4
		mov ebx, 1
		mov ecx, str_escrito
		mov edx, len_escrito
		int 0x80
		pop EAX ; volta valor antigo de EAX
		pop EDX ; volta valor antigo de EDX
		pop ECX ; volta valor antigo de ECX
		pop EBX ; volta valor antigo de EBX
		call output_function ; Mostra bytes escritos em EAX
		pop EAX ; volta valor antigo de EAX
		pop EBX ; volta valor antigo de EBX
		mov EAX, 1
		mov EBX, 0
		int 80h
input_function:
    xor ebx, ebx       ; ebx conta quantos bytes foram lidos, começa em zero
    xor edx, edx       ; edx será o acumulador para o número final, começa em zero
    call le_char       ; Chama a função que lê um caractere do usuário

    cmp cl, 0x0A       ; Compara o caractere lido com '\n' (0x0A)
    je fim             ; Se o caractere lido for '\n', pula para o fim para encerrar o programa
    cmp cl, '-'        ; Compara o caractere lido com '-' para verificar se é negativo
    je loop_negativo   ; Se for '-', pula para o loop de números negativos
    jmp positivo       ; Caso contrário, pula para o loop de números positivos

loop_positivo:
    call le_char       ; Chama a função para ler o próximo caractere
    cmp cl, 0x0A       ; Compara o caractere lido com '\n' (0x0A)
    je fim             ; Se o caractere lido for '\n', pula para o fim para encerrar o programa
    jmp positivo       ; Continua o processo de leitura e conversão de números positivos

loop_negativo:
    call le_char       ; Chama a função para ler o próximo caractere
    cmp cl, 0x0A       ; Compara o caractere lido com '\n' (0x0A)
    je fim             ; Se o caractere lido for '\n', pula para o fim para encerrar o programa
    jmp negativo       ; Continua o processo de leitura e conversão de números negativos

positivo:
    sub cl, '0'        ; Converte o caractere numérico em seu valor decimal
    movzx eax, cl      ; Move o valor decimal para eax
    imul edx, 10       ; Multiplica o acumulador (edx) por 10 para ajustar a posição do dígito
    add edx, eax       ; Adiciona o valor atual de eax ao acumulador edx
    jmp loop_positivo  ; Volta para o loop de leitura de números positivos

negativo:
    sub cl, '0'        ; Converte o caractere numérico em seu valor decimal
    movzx eax, cl      ; Move o valor decimal para eax
    imul edx, 10       ; Multiplica o acumulador (edx) por 10 para ajustar a posição do dígito
    sub edx, eax       ; Subtrai o valor atual de eax do acumulador edx
    jmp loop_negativo  ; Volta para o loop de leitura de números negativos

fim:
    ; O numero final está em edx!
    mov eax, ebx       ; Coloca o contador de bytes em eax tambem
    ret

le_char:
    ; Função para ler um caractere inserido pelo usuário
    ; Recebe o caractere do console e o retorna em cl
    ; Se o caractere lido for '\n', encerra o programa

    add ebx, 1

    push eax               ; Salva o valor atual de eax na pilha
    push ebx               ; Salva o valor atual de ebx na pilha
    push edx               ; Salva o valor atual de edx na pilha

    sub esp, 1             ; Reserva um byte na pilha
    mov eax, 3             ; syscall número 3: sys_read (chamada para leitura)
    mov ebx, 0             ; Ler da entrada padrão (stdin)
    lea ecx, [esp]         ; Aponta ecx para o local na pilha onde o byte será armazenado
    mov edx, 1             ; Define o tamanho do buffer como 1 byte
    int 0x80               ; Chamada de sistema para ler o caractere
    mov cl, byte [esp]     ; Coloca o byte lido em cl
    add esp, 1             ; Restaura o ponteiro da pilha
    pop edx                ; Restaura o valor original de edx da pilha
    pop ebx                ; Restaura o valor original de ebx da pilha
    pop eax                ; Restaura o valor original de eax da pilha

    ret                    ; Retorna da função le_char
output_function:
; Lê o numero em EBX e imprime
    xor ecx, ecx          ; Zera ECX, usado para contar o número de dígitos (inicialmente 0)
    cmp ebx, 0            ; Compara o valor de EBX com 0 para verificar se é negativo
    jge n_positivo        ; Se EBX for maior ou igual a 0 (não negativo), salta para o rótulo n_positivo

    mov eax, '-'          ; Se o número for negativo, coloca o caractere '-' em EAX
    call escreve_char     ; Chama a função escreve_char para imprimir o caractere '-'
    neg ebx               ; Torna EBX positivo (se estava negativo)
n_positivo:
    mov eax, ebx          ; Move o valor absoluto de EBX para EAX (preparando para conversão de dígitos)

convert_loop:
    xor edx, edx          ; Zera EDX, necessário para a operação de divisão
    mov ebx, 10           ; Carrega o divisor 10 em EBX para a divisão
    div ebx               ; Divide EAX por EBX (10), resultado em EAX (quociente) e EDX (resto)
    add ecx, 1            ; Incrementa o contador de dígitos (ECX) em 1
    add edx, '0'          ; Converte o resto (dígito) em seu valor ASCII
    push edx              ; Empilha o caractere ASCII na pilha
    cmp eax, 0            ; Compara o quociente (EAX) com 0
    jne convert_loop      ; Se o quociente não for 0, continua o loop para processar o próximo dígito

    mov ebx, ecx          ; Guarda em ebx os bytes escritos

    add ebx, 1            ; Guarda em ebx os bytes escritos

imprime_pilha:
    cmp ecx, 0            ; Compara o contador de dígitos (ECX) com 0
    je end                ; Se ECX for 0 (não há mais dígitos para imprimir), salta para o rótulo end
    pop eax               ; Remove o próximo caractere da pilha e coloca em EAX
    call escreve_char     ; Chama a função escreve_char para imprimir o caractere
    sub ecx, 1            ; Decrementa o contador de dígitos (ECX) em 1
    add eax, 1            ; incrementa o contador de bytes (EAX) em 1
    jmp imprime_pilha     ; Volta para o início do rótulo imprime_pilha para imprimir o próximo caractere

escreve_char:
    push ecx
    push ebx
    push edx
    push eax

    mov eax, 4            ; Define o número da syscall para sys_write (4)
    mov ebx, 1            ; Define o descritor de arquivo para stdout (1)
    mov ecx, esp          ; Define o ponteiro de dados (ECX) como o topo da pilha (contém o caractere a ser escrito)
    mov edx, 1            ; Define o número de bytes a serem escritos (1 byte)
    int 0x80              ; Chama o kernel para executar a syscall sys_write

    pop eax
    pop edx
    pop ebx
    pop ecx
    ret

end:
    push eax
    mov eax, 0x0A        ; Carrega o valor do caractere de nova linha (Line Feed) em EAX
    call escreve_char    ; Chama a função escreve_char para imprimir a nova linha
    pop eax              ; Preserva o valor de bytes escritos em eax
    mov eax,ebx          ; Guarda os bytes escritos + endline 
    ret                  ; Retorna da função output_function
