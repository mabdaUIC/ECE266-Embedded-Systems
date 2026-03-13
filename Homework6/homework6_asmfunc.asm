;****************************************************************
; homework6_asmfunc.asm – clean version (no delays)
;****************************************************************
                .cdecls "homework6.h"
                .text

                .global Copy4ByPreIndex
Copy4ByPreIndex .asmfunc
                LDR r2, [r1, #0]
                STR r2, [r0, #0]
                LDR r2, [r1, #4]
                STR r2, [r0, #4]
                LDR r2, [r1, #8]
                STR r2, [r0, #8]
                LDR r2, [r1, #12]
                STR r2, [r0, #12]
                BX  lr
                .endasmfunc

                .global Copy4ByPreIndexUpdate
Copy4ByPreIndexUpdate .asmfunc
                LDR r2, [r1]
                STR r2, [r0]
                LDR r2, [r1, #4]!
                STR r2, [r0, #4]!
                LDR r2, [r1, #4]!
                STR r2, [r0, #4]!
                LDR r2, [r1, #4]!
                STR r2, [r0, #4]!
                BX  lr
                .endasmfunc

                .global Copy4ByPostIndex
Copy4ByPostIndex .asmfunc
                LDR r2, [r1], #4
                STR r2, [r0], #4
                LDR r2, [r1], #4
                STR r2, [r0], #4
                LDR r2, [r1], #4
                STR r2, [r0], #4
                LDR r2, [r1], #4
                STR r2, [r0], #4
                BX  lr
                .endasmfunc

                .global Copy4ByRegOffset
Copy4ByRegOffset .asmfunc
                MOV r2, #0
                LDR r3, [r1, r2]
                STR r3, [r0, r2]
                ADD r2, #4
                LDR r3, [r1, r2]
                STR r3, [r0, r2]
                ADD r2, #4
                LDR r3, [r1, r2]
                STR r3, [r0, r2]
                ADD r2, #4
                LDR r3, [r1, r2]
                STR r3, [r0, r2]
                BX  lr
                .endasmfunc

                .global Swap2
Swap2           .asmfunc
                LSL r2, r2, #1
                ADD r0, r0, r2
                ADD r1, r1, r2
                LDRH r3, [r0]
                LDRH r12, [r1]
                STRH r12, [r0]
                STRH r3, [r1]
                BX  lr
                .endasmfunc
