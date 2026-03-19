; ras_asm.asm – ISR for rotary angle sensor
; Stores raw value in g_ras_raw and sets g_ras_ready flag.

        .cdecls "ras.h"

        .ref    g_ras_raw
        .ref    g_ras_ready
        .ref    ADCSequenceDataGet
        .ref    ADCIntClear

_ADC0_BASE      .field  0x40038068        ; ADC0 base address
_g_ras_raw      .field  g_ras_raw
_g_ras_ready    .field  g_ras_ready

        .text
        .thumb
        .global RasISR

RasISR:
        .asmfunc
        push    {lr}

        ; Read ADC result
        LDR     r0, _ADC0_BASE
        MOV     r1, #1                  ; sequencer 1
        SUB     sp, sp, #4
        MOV     r2, sp
        BL      ADCSequenceDataGet

        LDR     r1, [sp]
        ADD     sp, sp, #4

        ; Store raw value into g_ras_raw
        LDR     r0, _g_ras_raw
        STR     r1, [r0]

        ; Set ready flag
        LDR     r0, _g_ras_ready
        MOV     r1, #1
        STRB    r1, [r0]

        ; Clear interrupt
        LDR     r0, _ADC0_BASE
        MOV     r1, #1
        BL      ADCIntClear

        pop     {pc}
        .endasmfunc
