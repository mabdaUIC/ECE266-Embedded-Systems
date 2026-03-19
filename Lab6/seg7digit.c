#include <stdint.h>
#include <stdbool.h>
#include "seg7.h"

static uint8_t seg7_coding_table[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
};

void Seg7Update(Seg7Display *seg7)
{
    uint8_t seg7_code[4];
    uint8_t colon_code = seg7->colon_on ? 0b10000000 : 0;
    seg7_code[0] = seg7_coding_table[seg7->digit[0]] + colon_code;
    seg7_code[1] = seg7_coding_table[seg7->digit[1]] + colon_code;
    seg7_code[2] = seg7_coding_table[seg7->digit[2]] + colon_code;
    seg7_code[3] = seg7_coding_table[seg7->digit[3]] + colon_code;
    Seg7RawUpdate(seg7_code);
}
