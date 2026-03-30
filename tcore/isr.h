#ifndef ISR_H
#define ISR_H

#include <stdint.h>

/** @brief Estrutura que mapeia a pilha após o pusha e o stub assembly */
struct registers {
    uint32_t ds;                                     // Segmento de dados
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Registradores do pusha
    uint32_t int_no, err_code;                       // Empilhados manualmente
    uint32_t eip, cs, eflags, useresp, ss;           // Empilhados automaticamente pela CPU
};

/** @brief Protótipo do handler central de interrupções */
void isr_handler(struct registers regs);
void pic_remap();

#endif