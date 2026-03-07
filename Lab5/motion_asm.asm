; motion_asm.asm – Assembly functions for PIR motion sensor

        .cdecls "motion.h"
        .text

<<<<<<< HEAD
; Reference to the global motion sensor state variable
        .ref motion_sensor
_motion_sensor   .field motion_sensor
=======
		.ref	MotionStatus ;pointer to variable
_MotionStatus	.field	MotionStatus

; Declare the GPIO base adress for Port C (32-bit value)
_GPIO_PORTC_BASE		.field GPIO_PORTC_BASE
>>>>>>> 8eae2b7e9bef69a6d8f74fc9f265c13abd5a5074

;******************************************************************************
; C prototype: int32_t MotionRead()
; Reads the motion sensor input from the state variable.
; Returns non‑zero if motion detected, zero otherwise.
; Also clears the new_input flag.
;******************************************************************************
        .global MotionRead
        .asmfunc
MotionRead
<<<<<<< HEAD
        PUSH    {LR}

        ; Load address of motion_sensor
        LDR     r0, _motion_sensor

        ; Check new_input flag (byte at offset 8)
        LDRB    r1, [r0, #8]        ; r1 = new_input
        CMP     r1, #0
        BEQ     no_new_input

        ; There is new input: load raw_input (at offset 4)
        LDR     r2, [r0, #4]        ; r2 = raw_input

        ; Clear the new_input flag
        MOV     r1, #0
        STRB    r1, [r0, #8]        ; store 0 to new_input

        ; Return raw_input (in r0)
        MOV     r0, r2
        POP     {PC}

no_new_input
        MOV     r0, #0
        POP     {PC}
        .endasmfunc
=======
		PUSH	{LR}     ; save the return adress
		LDR	R1, _MotionStatus ;load MotionStatus

		;call GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_4)
		LDR		r0, _GPIO_PORTC_BASE
		MOV 	r1, #GPIO_PIN_4
		BL		GPIOPinRead
		LDR	R0, [R1, #0] ;load isMotionDetected

		; GPIOPinRead returns a bitmask with the requested pins.
		; If PC4 is high, the result will have bit 4 set (non-zero).
		; We can return it directly (the caller only cares about zero/non-zero).
		;clear the lfag after reading
		MOV	R2, #0
		STR	R2, [R1, #0]

		POP 	{PC} 		; Return
		BX	LR ;return
		.endasmfunc
>>>>>>> 8eae2b7e9bef69a6d8f74fc9f265c13abd5a5074
