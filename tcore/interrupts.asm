; Manipuladores de Interrupção de baixo nível (ISRs)
[extern isr_handler]

; Macro para ISRs que não empilham código de erro automaticamente pela CPU
%macro ISR_NOERRCODE 1
  global isr%1
  isr%1:
    push byte 0      ; Erro fictício para manter a pilha consistente
    push byte %1     ; Número da interrupção
    jmp isr_common_stub
%endmacro

; Macro para ISRs que já empilham código de erro (ex: 8, 10-14)
%macro ISR_ERRCODE 1
  global isr%1
  isr%1:
    push byte %1     ; Número da interrupção
    jmp isr_common_stub
%endmacro

; Gerar ISRs para as 32 exceções da CPU
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE   17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

; Stub comum para processar o estado da CPU e chamar o C
isr_common_stub:
    pusha            ; Salva EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX
    
    mov ax, ds       ; Salva o seletor de dados atual
    push eax

    mov ax, 0x10     ; Carrega o seletor de dados do Kernel (GDT 0x10)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler ; Chama o handler em C (tcore/isr.c)

    pop eax          ; Restaura o seletor de dados original
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa             ; Restaura registradores gerais
    add esp, 8       ; Limpa o número da interrupção e o código de erro
    iret             ; Retorno especial de interrupção