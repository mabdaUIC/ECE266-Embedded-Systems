#ifndef SEG7_H_
#define SEG7_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t digit[4];
    bool colon_on;
} Seg7Display;

void Seg7Init();
void Seg7RawUpdate(uint8_t code[]);
void Seg7Update(Seg7Display *seg7);

#endif
