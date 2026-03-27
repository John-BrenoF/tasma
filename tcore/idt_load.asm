; Disponibiliza a função idt_flush para o código C
global idt_flush

idt_flush:
    ; Carrega o endereço do ponteiro IDT (argumento da função) em EAX
    mov eax, [esp+4]
    ; Executa a instrução privilegiada para carregar a IDT no registrador da CPU
    lidt [eax]
    ret
