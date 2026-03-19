; ras_asm.asm
        .cdecls "ras.h"

        .ref    ras_state
        .ref    ADCSequenceDataGet
        .ref    ADCIntClear

_ras_state      .field  ras_state
_ADC0_BASE      .field  ADC0_BASE

        .text
        .asmfunc
RasISR
        push    {lr}

        ; Read ADC result into ras_state.raw_data
        LDR     r0, _ADC0_BASE
        MOV     r1, #1                ; sequencer 1
        LDR     r2, _ras_state
        ADD     r2, #4                 ; &ras_state.raw_data
        BL      ADCSequenceDataGet

        ; Set ras_state.new_input = true
        LDR     r0, _ras_state
        MOV     r1, #1
        STRB    r1, [r0, #8]           ; offset of new_input

        ; Clear interrupt
        LDR     r0, _ADC0_BASE
        MOV     r1, #1
        BL      ADCIntClear

        pop     {pc}
        .endasmfunc
