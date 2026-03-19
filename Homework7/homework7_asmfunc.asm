;****************************************************************
; homework7_asmfunc.asm: Assembly code for homework 7 assignment.
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
