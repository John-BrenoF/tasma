#include "isr.h"
#include "../tlib/tstring.h"

/** @brief Funções auxiliares para comunicação com portas I/O */
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

/** @brief Remapeia o PIC para que IRQs de hardware não colidam com exceções da CPU */
void pic_remap() {
    uint8_t mask1 = inb(0x21);
    uint8_t mask2 = inb(0xA1);

    outb(0x20, 0x11); // ICW1: Iniciar inicialização
    outb(0xA0, 0x11);

    outb(0x21, 0x20); // ICW2: Offset Master (Interrupções 32-39)
    outb(0xA1, 0x28); // ICW2: Offset Slave (Interrupções 40-47)

    outb(0x21, 0x04); // ICW3: Master tem escravo na IRQ2
    outb(0xA1, 0x02); // ICW3: Slave conectado à IRQ2

    outb(0x21, 0x01); // ICW4: Modo 8086
    outb(0xA1, 0x01);

    outb(0x21, mask1); // Restaura máscaras
    outb(0xA1, mask2);
}

/** @brief Handler central que recebe todas as exceções da CPU */
void isr_handler(struct registers regs) {
    tvga_clear();
    tvga_print("\n--- KERNEL PANIC ---\n", 0x0C);
    tvga_print("Excecao detectada: ", 0x07);
    
    // Como ainda não temos conversor de int para string, imprimimos o ID básico
    if (regs.int_no == 0) {
        tvga_print("DIVISION BY ZERO (0x00)", 0x0E);
    } else if (regs.int_no == 13) {
        tvga_print("GENERAL PROTECTION FAULT (0x0D)", 0x0E);
    } else {
        tvga_print("CPU EXCEPTION", 0x0E);
    }

    tvga_print("\n\nEstado do Processador:", 0x07);
    tvga_print("\nEIP: ", 0x07);
    // Registradores poderiam ser impressos aqui com um formatador hexadecimal
    
    tvga_print("\n\nSistema travado para proteger o hardware.", 0x04);

    while(1) {
        asm volatile("cli; hlt");
    }
}