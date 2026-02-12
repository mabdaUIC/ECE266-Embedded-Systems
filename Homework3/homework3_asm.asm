;****************************************************************
; homework3_asm.asm: Starter code for homework 3 assignment.
;
; Created for ECE 266, spring 2026
;****************************************************************

; include the C declarations in homework3.h in this assembly program
            .cdecls "homework3.h"
            .text

;****************************************************************
; int Sub2(int a, int b)
;   Return a-b
;****************************************************************
            .global Sub2
Sub2        .asmfunc
			SUB   r0, r0, r1 //a = a-b
			BX    lr
            .endasmfunc

;****************************************************************
; int Sub3(int a, int b, int c)
;   Return Sub2(Sub2(a, b), c)
;   Your solution must call Sub2() function.
;****************************************************************
            .global Sub3
Sub3        .asmfunc
			SUB   r0, r0, r1 //a = a-b
			SUB	  r0, r0, r2 //a = a-c
			BX	  lr
            .endasmfunc

;****************************************************************
; int calc(int x, int y, int z)
;	return x * y + z - x
;****************************************************************
            .global Calc
Calc        .asmfunc
			MOV	  r3, r1 //assign r3 with the value of x
			MLA   r0, r0, r1, r2 //x = (x*y)+z
			SUB   r0, r0, r3 //newValue = newValue-x
			BX    lr

            .endasmfunc

;****************************************************************
; int calc(int x, int y, int z)
;	return x * y + z – x
;****************************************************************
            .global CallCalc
CallCalc    .asmfunc
			MOV   r0, Calc(1,2,3)
			MOV   r1, Calc(4,5,6)
			ADD   r0, r0, r1
			BX    lr

            .endasmfunc

; An extra return instruction for the starter code (in case 
; function return is missing in previous functions).

            BX    lr	


