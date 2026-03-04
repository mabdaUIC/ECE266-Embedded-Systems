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
		LDR	R1, _MotionStatus ;load MotionStatus

		LDR	R0, [R1, #0] ;load isMotionDetected

		;clear the lfag after reading
		MOV	R2, #0
		STR	R2, [R1, #0]

		BX	LR ;return
		.endasmfunc
