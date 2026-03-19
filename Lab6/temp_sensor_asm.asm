; temp_sensor_asm.asm
        .cdecls "temp_sensor.h"

        .ref temp_sensor
        .ref ADCSequenceDataGet
        .ref ADCIntClear

_temp_sensor    .field      temp_sensor
_ADC0_BASE      .field      ADC0_BASE

        .text
        .asmfunc
TempSensorISR
        push    {lr}

        ; Read ADC result into temp_sensor.raw_data
        LDR     r0, _ADC0_BASE
        MOV     r1, #0                ; sequencer 0
        LDR     r2, _temp_sensor
        ADD     r2, #4                 ; &temp_sensor.raw_data
        BL      ADCSequenceDataGet

        ; Set temp_sensor.new_input = true
        LDR     r0, _temp_sensor
        MOV     r1, #1
        STRB    r1, [r0, #8]           ; offset of new_input

        ; Clear interrupt
        LDR     r0, _ADC0_BASE
        MOV     r1, #0
        BL      ADCIntClear

        pop     {pc}
        .endasmfunc
