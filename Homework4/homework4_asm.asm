;****************************************************************
; homework4_asm.asm: Assembly starter code for homework 4 assignment.
;
; Created by Zhao Zhang for ECE 266, spring 2026
; Muhammad Abdallah 656747437 2/18/26
;****************************************************************

; include the C declarations in homework5.h in this assembly program
                .cdecls "homework4.h"

                .text

;****************************************************************
; uint32_t clearBits7to4(uint32_t n);
;   Clear bits 7 to 4 of n
;****************************************************************
                .global ClearBits7to4
ClearBits7to4   .asmfunc

				BFC r0, #4, #4 ;starting from bit 4, clear 4 bits
				BX lr ;return to caller

                ; don't forget function return
                .endasmfunc

;****************************************************************
; uint32_t insertField12Bits(uint32_t n, uint32_t m);
;   Insert 12 bits of m (starting at bit 0) to n, starting at
;   bit 8 of n
;****************************************************************
                .global InsertField12Bits
InsertField12Bits .asmfunc

				BFI r0, r1, #8, #12 ;insert 12 lowest bits of r1 into r0 starting at bit 8 of r0
				BX lr ;return to caller

                ; don't forget function return
                .endasmfunc

;****************************************************************
; uint32_t reverseByte(uint32_t n)
;   Reverse byte ordering. Must use REV instruction.
;****************************************************************
                .global ReverseByte
ReverseByte     .asmfunc

				REV r0, r0 ;into r0, put the result of reversing r0
				BX lr ;return to caller

                ; don't forget function return
                .endasmfunc

;****************************************************************
; int32_t ssat10Bits(int32_t n);
;   saturate a 32-bit signed integer to 10-bit
;****************************************************************
                .global Ssat10Bits
Ssat10Bits      .asmfunc

				SSAT r0, #10, r0 ;stored in r0, it saturates the signed value in r0 to a 10 bit range
				BX lr ;return to caller

                ; don't forget function return
                .endasmfunc

;****************************************************************
;   uint32_t usat10Bits(uint32_t n)
;   saturate a 32-bit unsigned integer to 10-bit
;****************************************************************
                .global Usat10Bits
Usat10Bits      .asmfunc

				USAT r0, #10, r0 ;stored in r0, it saturates the unsigned value in r0 to a 10 bit range
				BX lr ;return to caller

                ; don't forget function return
                .endasmfunc

;****************************************************************
; uint32_t rev16ForByte(uint32_t n)
;   Reverse byte ordering within halfword.
;****************************************************************
                .global Rev16ForByte
Rev16ForByte    .asmfunc

				REV16 r0, r0 ;reverse the 4 byte order of r0 into r0
				BX lr ;return to caller

                ; don't forget function return
                .endasmfunc

;****************************************************************
; int32_t signExt8to32(int8_t n)
;   sign-extend from 8-bit to 32-bit.
;****************************************************************
                .global SignExt8to32
SignExt8to32    .asmfunc

				SXTB r0, r0 ;sign extends the lowest byte of r0
				BX lr ;return to caller

                ; don't forget function return
                .endasmfunc

;****************************************************************
; uint32_t rotateRight8(uint32_t n)
;   Rotate right by 8 bits.
;****************************************************************
                .global RotateRight8
RotateRight8    .asmfunc

				ROR r0, r0, #8 ;rotates r0 by 8
				BX lr ;return to caller

                .endasmfunc

;****************************************************************
; int32_t multiply17(int32_t x)
;   Multiply by 17, must NOT use any multiply instruction. Hint:
;   Use an ADD instruction with built-in shift.
;****************************************************************
                .global Multiply17
Multiply17      .asmfunc

				ADD r0, r0, r0, LSL #4 ;r0 = r0 + (r0 * 2^4) = r0*17
				BX lr ;return to caller

                .endasmfunc

; Do NOT forget the function return in each of the above functions.
;
; The following is a catch-up function return.  With your solution
; code, the program execution should not reach here.
                BX    lr
