;****************************************************************
; homework6_asmfunc.asm: Assembly code for homework 6 assignment.
;
; Created by Zhao Zhang for ECE 266, spring 2026
;****************************************************************

; include the C declarations in homework6.h in this assembly program
                .cdecls "homework6.h"

                .text

;****************************************************************
; copy4ByPreIndex(int Y[], int X[])
;   Copy from array X[] to Array Y[], the size is fixed at four.
;   Must use pre-index in all load/store instructions.
;****************************************************************
                .global Copy4ByPreIndex
Copy4ByPreIndex .asmfunc

	LDR	r2, [r1] ;load X[0]
	STR	r2, [r0] ;store to Y[0]
	LDR	r2, [r1, #4] ;load X[1]
	STR	r2, [r0, #4] ;store to Y[1]
	LDR	r2, [r1, #8] ;load X[2]
	STR	r2, [r0, #8] ;store to Y[2]
	LDR	r2, [r1, #12] ;load X[3]
	STR	r2, [r0, #12] ;store to Y[3]

                .endasmfunc

;****************************************************************
; copy4ByPreIndexUpdate(int Y[], int X[])
;   Copy from array X[] to Array Y[], the size is fixed at four.
;   Must use pre-index with update in most load/store instructions.
;****************************************************************
                .global Copy4ByPreIndexUpdate
Copy4ByPreIndexUpdate .asmfunc

	LDR	r2, [r1] ;load X[0]
	STR	r2, [r0] ;store to Y[0]
	LDR	r2, [r1, #4]! ;update r1 to r1+4 then load X[1]
	STR	r2, [r0, #4]! ;update r0 to r0+4 then store to Y[1]
	LDR r2,	[r1, #4]! ;update r1 then load X[2]
	STR r2, [r0, #4]! ;update r0 then store Y[2]
	LDR r2,	[r1, #4]! ;update r1 then load X[3]
	STR r2, [r0, #4]! ;update r0 then store Y[3]

                .endasmfunc

;****************************************************************
; copy4ByPostIndex(int Y[], int X[])
;   Copy from array X[] to Array Y[], the size is fixed at four.
;   Must use post-index in most load/store instructions.
;****************************************************************
                .global Copy4ByPostIndex
Copy4ByPostIndex .asmfunc

	LDR r2, [r1], #4 ;load X[0] then r1 += 4
	STR r2, [r0], #4 ;store Y[0] then r0 += 4
	LDR r2, [r1], #4 ;load X[1] then r1 += 4
	STR r2, [r0], #4 ;store Y[1] then r0 += 4
	LDR r2, [r1], #4 ;load X[2] then r1 += 4
	STR r2, [r0], #4 ;store Y[2] then r0 += 4
	LDR r2, [r1], #4 ;load X[3] then r1 += 4
	STR r2, [r0], #4 ;store Y[3] then r0 += 4

                .endasmfunc

;****************************************************************
; copy4ByRegOffset mult64(int Y[], int X[])
;   Copy from array X[] to Array Y[], the size is fixed at four.
;   Must use register offset in most load/store instructions.
;****************************************************************
                .global Copy4ByRegOffset
Copy4ByRegOffset .asmfunc

	MOV r2,	#0 ;initialize offset to 0
	LDR r3, [r1, r2] ;load X+r2
	STR r3, [r0, r2] ;store Y+r2
	MOV r2, #4
	LDR r3, [r1, r2]
	STR r3, [r0, r2]
	MOV r2, #8
	LDR r3, [r1, r2]
	STR r3, [r0, r2]
	MOV r2, #12
	LDR r3, [r1, r2]
	STR r3, [r0, r2]

                .endasmfunc

;****************************************************************
; void swap2(short X[], short Y[], int i)
;   Swap X[i] and Y[i]
;****************************************************************
                .global Swap2
Swap2           .asmfunc

	LDRH	r3, [r0, r2, LSL #1] ;r3 = X[i]
	LDRH	r4, [r0, r2, LSL #1] ;r3 = X[i]
	STRH	r3,	[r1, r2, LSL #1] ;Y[i] = r3
	STRH	r4,	[r0, r2, LSL #1] ;X[i] = r4

                .endasmfunc


	        	BX   lr		; an extra return instruction
	               			; for the starter code to return
