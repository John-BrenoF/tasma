#include <stdint.h>

// Estrutura de uma entrada na GDT (8 bytes)
struct gdt_entry_struct {
    uint16_t limit_low;           // Os 16 bits inferiores do limite
    uint16_t base_low;            // Os 16 bits inferiores da base
    uint8_t  base_middle;         // Os próximos 8 bits da base
    uint8_t  access;              // Determina o privilégio (Ring 0 ou 3)
    uint8_t  granularity;
    uint8_t  base_high;           // Os últimos 8 bits da base
} __attribute__((packed));

// Ponteiro para a GDT que a CPU vai ler
struct gdt_ptr_struct {
    uint16_t limit;               // Tamanho da tabela - 1
    uint32_t base;                // Endereço da primeira entrada
} __attribute__((packed));

struct gdt_entry_struct gdt_entries[5];
struct gdt_ptr_struct   gdt_ptr;

// Função externa em Assembly para aplicar a GDT
extern void gdt_flush(uint32_t);

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}

void tasma_gdt_init() {
    gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * 5) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment (Kernel)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment (Kernel)
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // Code segment (User)
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // Data segment (User)

    gdt_flush((uint32_t)&gdt_ptr);
}
