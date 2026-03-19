;****************************************************************
; homework7_asmfunc.asm: Assembly code for homework 7 assignment.
;
; Created by Zhao Zhang for ECE 266, spring 2026
;****************************************************************

; include the C declarations in homework7.h in this assembly program
                .cdecls "homework7.h"

                .text

;****************************************************************
; unsigned umax_cb(unsigned a, unsigned b)
;   Return the maximum of a and b. Use conditional branch.
;****************************************************************
                .global umax_cb
umax_cb         .asmfunc
	CMP	r0, r1
	BHS	skip_else
	MOV	r0, r1

skip_else
	BX	lr

                .endasmfunc


;****************************************************************
; unsigned umax_ce(unsigned a, unsigned b)
;   Return the maximum of a and b. Use conditional execution.
;****************************************************************
				.global umax_ce
umax_ce         .asmfunc
	CMP	r0, r1
	IT	LO
	MOVLO	r0, r1
	BX	lr

                .endasmfunc

;****************************************************************
; int sign_cb(int n)
;   Return sign -1, 0, 1 if n is negative, zero, or positive,
;   respectively. Use conditional branch.
;****************************************************************
                .global sign_cb
sign_cb         .asmfunc
	CMP	r0, #0
	BLT	is_negative
	BEQ	is_zero
	MOV	r0, #1
	BX	lr

is_negative
	MOV	r0, #-1
	BX	lr

is_zero
	MOV	r0, #0
	BX	lr


                .endasmfunc


;****************************************************************
; int sign_ce(int n)
;   Return sign -1, 0, 1 if n is negative, zero, or positive,
;   respectively. Use conditional execution.
;****************************************************************
                .global sign_ce
sign_ce         .asmfunc
	CMP	r0, #0
	IT	NE
	MOVNE	r0, #1
	IT	LT
	MOVLT	r0, #-1
	BX	lr

                .endasmfunc


;****************************************************************
; int swap_cb(unsigned short X[], int k)
;    If X[k] > X[k+1], swap X[k] and X[k+1] and return 1;
;    otherwise, return 0. Use conditional branch.
;****************************************************************
                .global swap_cb
swap_cb         .asmfunc
	ADD	r2, r0, r1, LSL #1
	LDRH	r3, [r2]
	LDRH	r1, [r2, #2]
	CMP	r3, r1
	BLS	skip_swap
	STRH	r1, [r2]
	STRH	r3, [r2, #2]
	MOV	r0, #1
	BX	lr

skip_swap
	MOV	r0, #0
	BX	lr

                .endasmfunc


;****************************************************************
; int swap_ce(unsigned short X[], int k)
;    If X[k] > X[k+1], swap X[k] and X[k+1] and return 1;
;    otherwise, return 0. Use conditional execution.
;****************************************************************
                .global swap_ce
swap_ce         .asmfunc
	CMP	r0, #0
	IT	GT
	MOVGT	r0, #1
	IT	LT
	MOVLT	r0, #-1
	BX	lr

                .endasmfunc


; Do NOT forget the function return in each of the above functions.
;
; The following is a catch-up function return.  With your solution
; code, the program execution should not reach here.
                BX    lr

