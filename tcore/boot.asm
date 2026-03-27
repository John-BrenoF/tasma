; TASMA-KERNEL Bootloader Entry
; Definições para o cabeçalho Multiboot (compatível com GRUB e QEMU)
MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1
FLAGS    equ  MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC    ; Número identificador para o bootloader
    dd FLAGS    ; Requisitos do kernel para o bootloader
    dd CHECKSUM ; Validação do cabeçalho

; Reserva espaço para a pilha (stack) do Kernel
section .bss
align 16
stack_bottom:
    resb 16384 ; Aloca 16 KiB para evitar Stack Overflow inicial
stack_top:

section .text
global _start:function (_start.end - _start)
_start:
    ; Configura o Stack Pointer (ESP) para o topo da memória reservada
    mov esp, stack_top

    ; Salta para o código principal escrito em C
    extern tasma_main
    call tasma_main

    ; Loop de segurança: se o kernel retornar, desliga interrupções e para a CPU
    cli
.hang:  hlt
    jmp .hang
.end:
