; ras_ioreg_asm.asm – Bonus: ISR for RAS using I/O register access
        .cdecls "ras.h"

        .ref    g_ras_raw
        .ref    g_ras_ready

_ADC0_BASE      .field  0x40038000
_SSFIFO1_OFFSET .field  0x068
_ISC_OFFSET     .field  0x00C
_IN1_MASK       .field  0x00000002

_g_ras_raw      .field  g_ras_raw
_g_ras_ready    .field  g_ras_ready

        .text
        .thumb
        .global RasISR

RasISR:
        .asmfunc
        push    {lr}

        ; Read ADC result from SSFIFO1
        LDR     r0, _ADC0_BASE
        LDR     r1, _SSFIFO1_OFFSET
        LDR     r2, [r0, r1]
        LDR     r0, _g_ras_raw
        STR     r2, [r0]

        ; Set ready flag
        LDR     r0, _g_ras_ready
        MOV     r1, #1
        STRB    r1, [r0]

        ; Clear interrupt (write 1 to bit 1 of ISC)
        LDR     r0, _ADC0_BASE
        LDR     r1, _ISC_OFFSET
        LDR     r2, _IN1_MASK
        STR     r2, [r0, r1]

        pop     {pc}
        .endasmfunc
