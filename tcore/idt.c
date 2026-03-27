#include <stdint.h>

/**
 * @brief Estrutura de uma entrada na IDT (Interrupt Descriptor Table).
 * 
 * Cada "Gate" define para onde o processador deve saltar quando ocorre 
 * uma interrupção de hardware ou exceção de software.
 */
struct idt_entry_struct {
    uint16_t base_low;    /**< Bits 0-15 do endereço da função (ISR) */
    uint16_t sel;         /**< Seletor de segmento de código na GDT (0x08) */
    uint8_t  always0;     /**< Reservado, deve ser zero */
    uint8_t  flags;       /**< Flags: Presente, DPL (privilégio), Tipo de Gate (0x8E) */
    uint16_t base_high;   /**< Bits 16-31 do endereço da função (ISR) */
} __attribute__((packed));

/**
 * @brief Estrutura usada para carregar a IDT no registrador IDTR da CPU.
 */
struct idt_ptr_struct {
    uint16_t limit;       /**< Tamanho da tabela em bytes - 1 */
    uint32_t base;        /**< Endereço de memória inicial da tabela */
} __attribute__((packed));

/** @brief Tabela com os 256 vetores possíveis no modo x86 */
struct idt_entry_struct idt_entries[256];
struct idt_ptr_struct   idt_ptr;

/** @brief Protótipo da função assembly (idt_load.asm) que executa 'lidt' */
extern void idt_flush(uint32_t);

/**
 * @brief Configura um gate individual na tabela IDT.
 */
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    // O endereço de 32 bits da função é dividido em dois campos de 16 bits
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags   = flags;
}

/**
 * @brief Inicializa a IDT e ativa o tratamento de interrupções na CPU.
 */
void tasma_idt_init() {
    // Calcula o limite e o endereço base para o ponteiro IDTR
    idt_ptr.limit = sizeof(struct idt_entry_struct) * 256 - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    // Limpa todas as entradas para evitar que interrupções não tratadas travem o sistema
    for(int i=0; i<256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // Chama a rotina Assembly para aplicar as configurações no processador
    idt_flush((uint32_t)&idt_ptr);
}
