#include <stdint.h>

/** @brief Configurações do modo de texto VGA padrão (80 colunas por 25 linhas) */
static const uint8_t COLUMNS = 80;
static const uint8_t ROWS = 25;

/** @brief Endereço base da memória de vídeo VGA para modo texto colorido */
static uint16_t* const VIDEO_MEMORY = (uint16_t*)0xB8000;

/** @brief Índice linear que rastreia a próxima posição de escrita na tela */
static int cursor_pos = 0;

/**
 * @brief Limpa toda a tela preenchendo-a com espaços vazios e cor padrão (cinza claro)
 */
void tvga_clear() {
    for (int i = 0; i < COLUMNS * ROWS; i++) {
        // 0x07 define cinza sobre preto. O caractere ocupa o byte baixo e a cor o alto.
        VIDEO_MEMORY[i] = (uint16_t)' ' | (uint16_t)0x07 << 8;
    }
    cursor_pos = 0;
}

/**
 * @brief Imprime uma string na tela processando quebras de linha e cores
 */
void tvga_print(const char* str, uint8_t color) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            // Pula para a próxima linha
            cursor_pos += COLUMNS - (cursor_pos % COLUMNS);
        } else {
            // Combina o caractere ASCII com o atributo de cor
            VIDEO_MEMORY[cursor_pos++] = (uint16_t)str[i] | (uint16_t)color << 8;
        }
        
        // Proteção simples: se atingir o fim da tela, volta ao início (topo)
        if (cursor_pos >= COLUMNS * ROWS) {
            cursor_pos = 0; 
        }
    }
}
