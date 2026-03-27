#include <stdint.h>

static const uint8_t COLUMNS = 80;
static const uint8_t ROWS = 25;
static uint16_t* const VIDEO_MEMORY = (uint16_t*)0xB8000;
static int cursor_pos = 0;

void tvga_clear() {
    for (int i = 0; i < COLUMNS * ROWS; i++) {
        VIDEO_MEMORY[i] = (uint16_t)' ' | (uint16_t)0x07 << 8;
    }
    cursor_pos = 0;
}

void tvga_print(const char* str, uint8_t color) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            // Pula para a próxima linha
            cursor_pos += COLUMNS - (cursor_pos % COLUMNS);
        } else {
            VIDEO_MEMORY[cursor_pos++] = (uint16_t)str[i] | (uint16_t)color << 8;
        }
        
        // Impede que o cursor saia da tela (scroll simples)
        if (cursor_pos >= COLUMNS * ROWS) {
            cursor_pos = 0; // Por enquanto volta ao topo
        }
    }
}
