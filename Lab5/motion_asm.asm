; motion_asm.asm – Assembly functions for PIR motion sensor
; Modified for interrupt‑based operation

        .cdecls "motion.h"          ; include C declarations
        .text

; Reference to the global motion sensor state variable
        .ref motion_sensor
_motion_sensor   .field motion_sensor

;******************************************************************************
; C prototype: int32_t MotionRead()
; Reads the motion sensor input from the state variable.
; Returns non‑zero if motion detected, zero otherwise.
; Also clears the new_input flag.
;******************************************************************************
        .global MotionRead
        .asmfunc
MotionRead
        PUSH    {LR}                ; save return address

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
