/*
 * homework4.h: Starter code for homework 4
 *
 * ECE 266, spring 2026
 * Created by Zhao Zhang
 */

#ifndef HOMEWORK4_H_
#define HOMEWORK4_H_

#include <stdint.h>

// 64-bit subtract
uint32_t C_ClearBits7to4(uint32_t n);
uint32_t ClearBits7to4(uint32_t n);

// Insert 12 bits of m (starting at bit 0) to n, starting at bit 8 of n
uint32_t C_InsertField12Bits(uint32_t n, uint32_t m);
uint32_t InsertField12Bits(uint32_t n, uint32_t m);

// reverse byte ordering
uint32_t ReverseByte(uint32_t n);
uint32_t C_ReverseByte(uint32_t n);

// saturate a 32-bit signed integer to 10-bit
int32_t C_Ssat10Bits(int32_t n);
int32_t Ssat10Bits(int32_t n);

// saturate a 32-bit unsigned integer to 10-bit
uint32_t C_Usat10Bits(uint32_t n);
uint32_t Usat10Bits(uint32_t n);

// reverse byte ordering for halfwords within word
uint32_t Rev16ForByte(uint32_t n);
uint32_t C_Rev16ForByte(uint32_t n);

// sign-extend 8-bit to 32-bit
int32_t SignExt8to32(int32_t n);
int32_t C_SignExt8to32(int32_t n);

// rotate right by 8 bit
uint32_t RotateRight8(uint32_t n);
uint32_t C_RotateRight8(uint32_t n);

// multiply by 17 without using multiply
int32_t Multiply17(int32_t x);
int32_t C_Multiply17(int32_t x);

#endif /* HOMEWORK4_H_ */
