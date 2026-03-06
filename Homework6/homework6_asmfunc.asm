;****************************************************************
; homework6_asmfunc.asm: Assembly code for homework 7 assignment.
;
; Created by Zhao Zhang for ECE 266, spring 2026
;****************************************************************

; include the C declarations in homework7.h in this assembly program
                .cdecls "homework7.h"

                .text

;****************************************************************
; copy4ByPreIndex(int Y[], int X[])
;   Copy from array X[] to Array Y[], the size is fixed at four.
;   Must use pre-index in all load/store instructions.
;****************************************************************
                .global Copy4ByPreIndex
Copy4ByPreIndex .asmfunc

                .endasmfunc

;****************************************************************
; copy4ByPreIndexUpdate(int Y[], int X[])
;   Copy from array X[] to Array Y[], the size is fixed at four.
;   Must use pre-index with update in most load/store instructions.
;****************************************************************
                .global Copy4ByPreIndexUpdate
Copy4ByPreIndexUpdate .asmfunc

                .endasmfunc

;****************************************************************
; copy4ByPostIndex(int Y[], int X[])
;   Copy from array X[] to Array Y[], the size is fixed at four.
;   Must use post-index in most load/store instructions.
;****************************************************************
                .global Copy4ByPostIndex
Copy4ByPostIndex .asmfunc

                .endasmfunc

;****************************************************************
; copy4ByRegOffset mult64(int Y[], int X[])
;   Copy from array X[] to Array Y[], the size is fixed at four.
;   Must use register offset in most load/store instructions.
;****************************************************************
                .global Copy4ByRegOffset
Copy4ByRegOffset .asmfunc

                .endasmfunc

;****************************************************************
; void swap2(short X[], short Y[], int i)
;   Swap X[i] and Y[i]
;****************************************************************
                .global Swap2
Swap2           .asmfunc

                .endasmfunc


	        BX   lr		; an extra return instruction
	               		; for the starter code to return
