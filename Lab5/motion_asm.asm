;******************************************************************************
; motion_asm.asm: Assembly functions for PIR motion sensor
;
; ECE 266, spring 2026
; Lab 4
;******************************************************************************

		.cdecls "motion.h" ; to include the C declaration
		.text

		.ref	MotionStatus ;pointer to variable
_MotionStatus	.field	MotionStatus

; Declare the GPIO base adress for Port C (32-bit value)
_GPIO_PORTC_BASE		.field GPIO_PORTC_BASE

;******************************************************************************
; C prototype: int32_t MotionRead()
; Reads the motion sensor input on PC4.
; Returns non-zero if motion is detected, zero otherwise.
;******************************************************************************

		.global MotionRead
		.asmfunc
MotionRead
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
