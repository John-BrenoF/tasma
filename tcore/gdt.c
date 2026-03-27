#include <stdint.h>

/**
 * @brief Estrutura de uma entrada (Segment Descriptor) na GDT.
 * Define o endereço base, limite e permissões de um segmento de memória.
 */
struct gdt_entry_struct {
    uint16_t limit_low;           /**< Bits 0-15 do tamanho do segmento */
    uint16_t base_low;            /**< Bits 0-15 do endereço inicial */
    uint8_t  base_middle;         /**< Bits 16-23 do endereço inicial */
    uint8_t  access;              /**< Flags de acesso (Ex: Ring 0, Código/Dados) */
    uint8_t  granularity;         /**< Configura escala do limite e bits 16-19 do limite */
    uint8_t  base_high;           /**< Bits 24-31 do endereço inicial */
} __attribute__((packed));

/**
 * @brief Estrutura do ponteiro GDT que será carregado no registrador GDTR.
 */
struct gdt_ptr_struct {
    uint16_t limit;               /**< Tamanho total da tabela menos 1 byte */
    uint32_t base;                /**< Endereço linear do início da tabela */
} __attribute__((packed));

// Definimos 5 segmentos: Nulo, Código Kernel, Dados Kernel, Código User e Dados User.
struct gdt_entry_struct gdt_entries[5];
struct gdt_ptr_struct   gdt_ptr;

/** @brief Função em Assembly que ativa a GDT carregando os registradores de segmento */
extern void gdt_flush(uint32_t);

/** @brief Preenche os bits de uma entrada da GDT de forma organizada */
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}

/**
 * @brief Configura a tabela GDT para um modelo de memória "Flat" (plano).
 */
void tasma_gdt_init() {
    // Define o tamanho da tabela e aponta para a memória das entradas
    gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * 5) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment (Kernel)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment (Kernel)
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // Code segment (User)
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // Data segment (User)

    gdt_flush((uint32_t)&gdt_ptr);
}
