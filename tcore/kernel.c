#include "../tlib/tstring.h"
#include "isr.h"

/** @brief Funções de inicialização do núcleo (core) */
void tasma_gdt_init();
void tasma_idt_init();

/**
 * @brief Ponto de entrada do Kernel TASMA.
 */
void tasma_main() {
    // 1. Configura a segmentação de memória
    tasma_gdt_init();
    // 2. Configura a tabela de interrupções e exceções
    tasma_idt_init(); 

    // 3. Remapeia o PIC para evitar conflitos com IRQs de hardware
    pic_remap();

    tvga_clear(); // Limpa a tela inicial
    tvga_print("TASMA v1.0\n", 0x0A);
    tvga_print("Protecao GDT: [OK]\n", 0x07);
    tvga_print("Sistema IDT:  [OK]\n", 0x07);
    tvga_print("__________________________________", 0x07);

    tvga_print("Testando excecao: Division by Zero...\n", 0x0E);
    
    // Força uma exceção de divisão por zero para testar o handler
    int n = 0;
    int result = 10 / n;
    (void)result;

    while(1);
}
