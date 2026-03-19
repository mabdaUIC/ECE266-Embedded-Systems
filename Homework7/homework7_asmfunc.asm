;****************************************************************
; homework7_asmfunc.asm: Assembly code for homework 7 assignment.
<<<<<<< HEAD
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

=======
;****************************************************************
                .cdecls "homework7.h"
                .text

;*************** umax_cb (conditional branch) ******************
                .global umax_cb
umax_cb         .asmfunc
                CMP     R0, R1
                BHS     .L_return_a
                MOV     R0, R1
.L_return_a     BX      LR
                .endasmfunc

;*************** umax_ce (conditional execution) ***************
                .global umax_ce
umax_ce         .asmfunc
                CMP     R0, R1
                IT      LO
                MOVLO   R0, R1
                BX      LR
                .endasmfunc

;**************** sign_cb (conditional branch) *****************
                .global sign_cb
sign_cb         .asmfunc
                CMP     R0, #0
                BLT     .L_neg
                BEQ     .L_zero
                MOV     R0, #1
                B       .L_done
.L_neg          MOV     R0, #-1
                B       .L_done
.L_zero         MOV     R0, #0
.L_done         BX      LR
                .endasmfunc

;**************** sign_ce (conditional execution) ***************
                .global sign_ce
sign_ce         .asmfunc
                CMP     R0, #0
                IT      LT
                MOVLT   R0, #-1
                IT      EQ
                MOVEQ   R0, #0
                IT      GT
                MOVGT   R0, #1
                BX      LR
                .endasmfunc

;**************** swap_cb (conditional branch) *****************
                .global swap_cb
swap_cb         .asmfunc
                LSL     R2, R1, #1
                ADD     R2, R0, R2
                LDRH    R3, [R2]
                LDRH    R1, [R2, #2]
                CMP     R3, R1
                BLS     .L_no_swap
                STRH    R1, [R2]
                STRH    R3, [R2, #2]
                MOV     R0, #1
                BX      LR
.L_no_swap      MOV     R0, #0
                BX      LR
                .endasmfunc

;**************** swap_ce (conditional execution) ***************
                .global swap_ce
swap_ce         .asmfunc
                LSL     R2, R1, #1
                ADD     R2, R0, R2
                LDRH    R3, [R2]
                LDRH    R1, [R2, #2]
                CMP     R3, R1
                ITTTE   HI
                STRHHI  R1, [R2]
                STRHHI  R3, [R2, #2]
                MOVHI   R0, #1
                MOVLS   R0, #0
                BX      LR
                .endasmfunc
>>>>>>> a84fa3336c2bcb2d3d96b4c53ae3efc1a299eea9
