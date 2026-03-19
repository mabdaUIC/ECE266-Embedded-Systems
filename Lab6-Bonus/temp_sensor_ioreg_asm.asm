; temp_sensor_asm.asm
; Lab 6, ECE 266, spring 2026
;
; Assembly code for the temperature sensor ISR
; This version uses ADC I/O registers, with no TivaWare function call.
;
; It is more efficient, though more cumbersome in programming,
; to access I/O registers than using TivaWare.
;
; Last update: Spring 2026

        ; To include names declared in C
        .cdecls "temp_sensor.h"

		; Declare reference to temp_sensor in temp_sensor.c, with the
		; following data structure:
		; typedef struct {
    	;	  Event *callback_event;
    	;     uint32_t raw_data;
    	;     bool new_input;
    	; } TempSensorState;
    	.ref 	temp_sensor

        .text
       	; fields to hold pointers
_temp_sensor	.field 		temp_sensor
_ADC0_BASE		.field 		ADC0_BASE

;******************************************************************************
; ISR for reading temperature data. It is called when ADC conversion is done.
; This is the bonus part that does not call any TivaWare function
;******************************************************************************
TempSensorISR
        .asmfunc
        push    {lr}

        ; Read the ADC data from the SSFIFO0 data register
        ; In C: temp_sensor.raw_data = *(uint32_t*)(ADC0_BASE + ADC_O_SSFIFO0)
        LDR     r0, _ADC0_BASE				; load ADC0 base address
        LDR		r1, _temp_sensor			; load temp_sensor address
        LDR 	r2, [r0, #ADC_O_SSFIFO0]	; load ADC reading from ADC0 FIFO0 register
		STR		r2, [r1, #4] 				; store to temp_sensor.raw_data

        ; Set the flag for new data ready
        ; temp_sensor.new_input = true;
        MOV     r2, #true
        STRB    r2, [r1, #8]				; temp_sensor.new_input = true

        ; Schedule a data processing event
        ; call EventSchedule(temp_sensor.callback_event, EventGetCurrentTime());
        BL		EventGetCurrentTime
        MOV		r1, r0						; r1 = return value of EventGetCurrentTime()
        LDR		r0, _temp_sensor			; r0 = &temp_sensor
        LDR		r0, [r0]					; r0 = temp_sensor.callback_event
        BL		EventSchedule				; call EventSchedule()

        ; IMPORTANT: Clear the interrupt status for ADC0 sequencer #0
        ; Write 1 to the IN0 bit of the ISC register
        ; In C: *(uint32_t*)(ADC0_BASE + ADC_O_ISC) = ADC_ISA_IN0
        LDR		r0, _ADC0_BASE				; r0 = ADC0 base address
		MOV		r1, #ADC_ISC_IN0			; bit mask to clean interrupt status for sequencer #0
		STR		r1, [r0, #ADC_O_ISC]		; write to ISC (interrupt status and clear) register

        pop     {pc}
        .endasmfunc

