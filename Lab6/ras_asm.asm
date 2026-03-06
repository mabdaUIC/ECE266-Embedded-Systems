; ras_asm.asm  –  ECE 266 Lab 6, spring 2026
;
; Assembly ISR for the Rotary Angle Sensor (ADC0, sequencer #1).
; Fires when the ADC conversion on sequencer #1 completes.
;
; RasState layout in ras.c  (offsets used below):
;   +0  callback_event  (Event *)   4 bytes
;   +4  raw_data        (uint32_t)  4 bytes
;   +8  new_input       (bool)      1 byte
;
; NOTE: ras.h now includes <inc/hw_memmap.h> and <driverlib/adc.h> so that
; ADC0_BASE and other constants are visible to this file via .cdecls.

        .cdecls  "ras.h"        ; pulls in ADC0_BASE, ADC driverlib symbols, etc.
        .ref     ras_state      ; global RasState struct defined in ras.c

        .text

; Linker-resolved 32-bit address constants (inside .text for PC-relative LDR)
_ras_state  .field  ras_state
_ADC0_BASE  .field  ADC0_BASE

;==============================================================================
RasISR
        .asmfunc
        push    {lr}

        ; 1. Read the ADC FIFO into ras_state.raw_data
        ;    ADCSequenceDataGet(ADC0_BASE, 1, &ras_state.raw_data)
        LDR     r0, _ADC0_BASE          ; arg0 = ADC0 base address
        MOV     r1, #1                  ; arg1 = sequencer #1
        LDR     r2, _ras_state          ; r2   = &ras_state
        ADD     r2, r2, #4              ; arg2 = &ras_state.raw_data  (offset +4)
        BL      ADCSequenceDataGet

        ; 2. Set ras_state.new_input = true  (offset +8, 1-byte bool)
        LDR     r0, _ras_state
        MOV     r1, #1
        STRB    r1, [r0, #8]

        ; 3. Schedule the data-processing callback at the current time
        ;    EventSchedule(ras_state.callback_event, EventGetCurrentTime())
        BL      EventGetCurrentTime     ; r0 = current time (ms)
        MOV     r1, r0                  ; arg1 = time
        LDR     r0, _ras_state
        LDR     r0, [r0, #0]            ; arg0 = ras_state.callback_event (offset +0)
        BL      EventSchedule

        ; 4. Clear the ADC interrupt flag — MUST happen before returning
        ;    ADCIntClear(ADC0_BASE, 1)
        LDR     r0, _ADC0_BASE
        MOV     r1, #1
        BL      ADCIntClear

        pop     {pc}
        .endasmfunc
