#include "../tlib/tstring.h"

void tasma_main() {
    tvga_clear();
    
    // Cor 0x0A é o Verde Lima clássico de terminais
    tvga_print("TASMA v1.0\n", 0x0A);
    
    // Opcional: Uma linha separadora minimalista
    tvga_print("----------", 0x07); 

    while(1);
}
