/*
 * seg7.c: Starter code for the digital interface for the 4-digit 7-segment display.
 *
 * Created by Zhao Zhang for ECE 266 labs, spring 2024.
 *
 * Note: It is revised from the 7-segment code from previous semesters.
 */

#include <stdint.h>
#include <stdbool.h>
#include "seg7.h"

/*
 * 7-segment coding table. See https://en.wikipedia.org/wiki/Seven-segment_display. The segments
 * are named as A, B, C, D, E, F, G. In this coding table, segments A-G are mapped to bits 0-7.
 * Bit 7 is not used in the coding. This display uses active high signal, in which '1' turns ON a
 * segment, and '0' turns OFF a segment.
 *
 * Note: The table is incomplete. Fill the codes for digits 4-9.
 */
static uint8_t seg7_coding_table[11] = {
        0b00111111,         // digit 0
        0b00000110,         // digit 1
        0b01011011,         // digit 2
        0b01001111,         // digit 3
        0b01100110,         // digit 4
        0b01101101,         // digit 5
        0b01111101,         // digit 6
        0b00000111,         // digit 7
        0b01111111,         // digit 8
        0b01101111,         // digit 9
        0b00000000          //digit 10

};

/*
 * Update the 7-segment display using the Seg7Display structure.
 */
void Seg7Update(Seg7Display *seg7)
{
    uint8_t seg7_code[4];
    uint8_t colon_code;

    // Get the raw encoding for the colon
    colon_code = seg7->colon_on ? 0b10000000 : 0b00000000;

    // Get the raw encoding for 7-segment digits
    seg7_code[0] = seg7_coding_table[seg7->digit[0]];
    seg7_code[1] = seg7_coding_table[seg7->digit[1]] | colon_code;
    seg7_code[2] = seg7_coding_table[seg7->digit[2]];
    seg7_code[3] = seg7_coding_table[seg7->digit[3]];


    Seg7RawUpdate(seg7_code);
}

void Seg7DisplayInt(uint32_t value, bool colon_on) {
    static Seg7Display seg7;
    seg7.colon_on = colon_on;

    // Extract digits: thousands, hundreds, tens, ones
    seg7.digit[0] = (value / 1000) % 10;
    seg7.digit[1] = (value / 100) % 10;
    seg7.digit[2] = (value / 10) % 10;
    seg7.digit[3] = value % 10;

    // --- Leading Zero Blanking (Requirement) ---
    if (seg7.digit[0] == 0) {
        seg7.digit[0] = 10; // Use your blank code at index 10
        if (seg7.digit[1] == 0) {
            seg7.digit[1] = 10;
            if (seg7.digit[2] == 0) {
                seg7.digit[2] = 10;
            }
        }
    }

    Seg7Update(&seg7);
}

void Seg7DisplayTime(uint32_t left, uint32_t right, bool colon_on) {
    static Seg7Display seg7;
    seg7.colon_on = colon_on;

    // Left two digits (Feet)
    seg7.digit[0] = (left / 10) % 10;
    seg7.digit[1] = left % 10;

    // Right two digits (Inches)
    seg7.digit[2] = (right / 10) % 10;
    seg7.digit[3] = right % 10;

    // Optional: Blank leading zero for feet only
    if (seg7.digit[0] == 0) seg7.digit[0] = 10;

    Seg7Update(&seg7);
}
