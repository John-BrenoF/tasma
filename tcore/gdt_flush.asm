global gdt_flush
gdt_flush:
    mov eax, [esp+4]  ; Pega o ponteiro da GDT passado como argumento
    lgdt [eax]        ; Carrega a nova GDT

    mov ax, 0x10      ; 0x10 é o offset do nosso segmento de dados na GDT
    mov ds, ax        ; Atualiza todos os registradores de segmento
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush   ; Far jump para atualizar o CS (Code Segment)
.flush:
    ret
