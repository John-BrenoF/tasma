#include "../tlib/tstring.h"

// Protótipo da função de inicialização da GDT
void tasma_gdt_init();

void tasma_main() {
    // 1. Inicializa a GDT imediatamente para segurança
    tasma_gdt_init();

    // 2. Limpa a tela e mostra o cabeçalho minimalista
    tvga_clear();
    tvga_print("TASMA v1.0\n", 0x0A);
    tvga_print("Protecao GDT: [OK]\n", 0x07);
    tvga_print("----------", 0x07);

    while(1);
}
