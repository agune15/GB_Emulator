#include "stdio.h"
#include "gpu_debug.h"
#include "memory.h"

#define VRAM_START  0x8000
#define VRAM_END    0x9FFF

void print_VRAM_content(void) {
    int c_byte_addr = VRAM_START;
    int row_bytes = 0;

    while (c_byte_addr <= VRAM_END) {
        if (c_byte_addr % 0x100 == 0)
            printf("%4X\n", c_byte_addr);

        if (row_bytes < 16) {
            printf("%02X ", read_byte(c_byte_addr));
            row_bytes++;
        }

        if (row_bytes == 16) {
            printf("\n");
            row_bytes = 0;
        }

        c_byte_addr++;
    }
}

