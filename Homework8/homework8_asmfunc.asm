;****************************************************************
; homework8.asm: Assembly starter code for homework 8 assignment.
;
; Created by Zhao Zhang for ECE 266, spring 2024
;****************************************************************

; include the C declarations in homework8.h in this assembly program
                .cdecls "homework8.h"

                .text

;****************************************************************
; void copy1(short X[], short Y[], int N);
;****************************************************************
copy1           .asmfunc

copy1:
	CMP		r2, #0
	BLE		Done

loop:
	LDRH	r3, [r0], #2
	STRH	r3, [r1], #2

	SUBS	r2, r2, #1
	BGT		loop

Done:
	BX		LR

                .endasmfunc

;****************************************************************
; void sat12Array(short X[], int N)
;   Saturate every element of X[] to 12 bits.
;****************************************************************
                .global sat12Array
Ssat12Array     .asmfunc

Ssat12Array:
	CMP		r1, #0
	BLE		Done

Loop:
	LDRSH	r2, [r0]
	SSAT	r2, #12, r2
	STRH	r2, [r0], #2
	SUBS	r1, r1, #1
	BGT		Loop

Done:
	BX		LR

                .endasmfunc

;****************************************************************
; int dotProduct(short X[], short Y[], int N)
;   Calculate the dot product of X[] and Y[].
;****************************************************************
                .global dotProduct
DotProduct      .asmfunc

DotProduct:
	MOV		r3, #0
	CMP		r2, #0
	BLE		Done

Loop:
	LDRSH	r4, [r0], #2
	LDRSH	r5, [r1], #2
	MLA		r3, r4, r5, r3
	SUBS	r2, r2, #1
	BGT		Loop

Done:
	MOV		r0, r3
	BX		LR

                .endasmfunc


;****************************************************************
; int countChar(const char *str, char ch)
;   Count the occurance of ch in str.
;****************************************************************
                .global countChar
CountChar      	.asmfunc

CountChar:
	MOV		r3, #0

Loop:
	LDRB	r2, [r0], #1
	CBZ		r2, Done
	CMP		r2, r1
	IT		EQ
	ADDEQ	r3, r3, #1
	B		Loop

Done:
	MOV		r0, r3
	BX		LR


; Do NOT forget the function return in each of the above functions.
; The following is a catch-up function return.  With your solution
; code, the program execution should not reach here.
                BX    lr

