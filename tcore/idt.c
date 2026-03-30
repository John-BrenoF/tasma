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

/** @brief Declaração das ISRs definidas no assembly (interrupts.asm) */
extern void isr0();  extern void isr1();  extern void isr2();  extern void isr3();
extern void isr4();  extern void isr5();  extern void isr6();  extern void isr7();
extern void isr8();  extern void isr9();  extern void isr10(); extern void isr11();
extern void isr12(); extern void isr13(); extern void isr14(); extern void isr15();
extern void isr16(); extern void isr17(); extern void isr18(); extern void isr19();
extern void isr20(); extern void isr21(); extern void isr22(); extern void isr23();
extern void isr24(); extern void isr25(); extern void isr26(); extern void isr27();
extern void isr28(); extern void isr29(); extern void isr30(); extern void isr31();

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

    for(int i=0; i<256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // Registrar as 32 exceções principais (0x08 é o seletor de código na GDT, 0x8E é Interrupt Gate)
    idt_set_gate(0,  (uint32_t)isr0,  0x08, 0x8E);
    idt_set_gate(1,  (uint32_t)isr1,  0x08, 0x8E);
    idt_set_gate(2,  (uint32_t)isr2,  0x08, 0x8E);
    idt_set_gate(3,  (uint32_t)isr3,  0x08, 0x8E);
    idt_set_gate(4,  (uint32_t)isr4,  0x08, 0x8E);
    idt_set_gate(5,  (uint32_t)isr5,  0x08, 0x8E);
    idt_set_gate(6,  (uint32_t)isr6,  0x08, 0x8E);
    idt_set_gate(7,  (uint32_t)isr7,  0x08, 0x8E);
    idt_set_gate(8,  (uint32_t)isr8,  0x08, 0x8E);
    idt_set_gate(9,  (uint32_t)isr9,  0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    // Chama a rotina Assembly para aplicar as configurações no processador
    idt_flush((uint32_t)&idt_ptr);
}
