        .cdecls "temp_sensor.h"
        .ref    g_temp_raw
        .ref    g_temp_ready
        .ref    ADCSequenceDataGet
        .ref    ADCIntClear

_ADC0_BASE      .field  0x40038000
_g_temp_raw     .field  g_temp_raw
_g_temp_ready   .field  g_temp_ready

        .text
        .thumb
        .global TempSensorISR
TempSensorISR:
        push    {lr}
        LDR     r0, _ADC0_BASE
        MOV     r1, #0
        SUB     sp, sp, #4
        MOV     r2, sp
        BL      ADCSequenceDataGet
        LDR     r1, [sp]
        ADD     sp, sp, #4
        LDR     r0, _g_temp_raw
        STR     r1, [r0]
        LDR     r0, _g_temp_ready
        MOV     r1, #1
        STRB    r1, [r0]
        LDR     r0, _ADC0_BASE
        MOV     r1, #0
        BL      ADCIntClear
        pop     {pc}
