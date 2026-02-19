/*
 * homework4_cfunc.c: C functions used in this homework.
 *
 * ECE 266, spring 2026, created by Zhao Zhang
 */

#include <stdint.h>
#include "homework4.h"

// Clear bits 7-4 of n
uint32_t C_ClearBits7to4(uint32_t n)
{
    return n & 0xFFFFFF0F;
}

// Insert 12 bits of m (starting at bit 0) to n, starting at bit 8 of n
uint32_t C_InsertField12Bits(uint32_t n, uint32_t m)
{
    return (n & 0xFFF000FF) | ((m & 0x00000FFF) << 8);
}

// reverse byte ordering
uint32_t C_ReverseByte(uint32_t n)
{
    uint32_t byte0 = (n & 0x000000FF) << 24;
    uint32_t byte1 = (n & 0x0000FF00) << 8;
    uint32_t byte2 = (n & 0x00FF0000) >> 8;
    uint32_t byte3 = (n & 0xFF000000) >> 24;

    return byte0 | byte1 | byte2 | byte3;
}

// saturate a 32-bit signed integer to 10-bit
int32_t C_Ssat10Bits(int32_t n)
{
    if (n < -512)
    {
        return -512;
    }
    else if (n > 511)
    {
        return 511;
    }
    else
    {
        return n;
    }
}

// saturate a 32-bit unsigned integer to 10-bit
uint32_t C_Usat10Bits(uint32_t n)
{
    if (n > 1023)
    {
        return 1023;
    }
    else
    {
        return n;
    }
}

// reverse byte ordering for halfwords within word
uint32_t C_Rev16ForByte(uint32_t n)
{
    uint32_t byte0 = (n & 0x0000FF00) >> 8;
    uint32_t byte1 = (n & 0x000000FF) << 8;
    uint32_t byte2 = (n & 0xFF000000) >> 8;
    uint32_t byte3 = (n & 0x00FF0000) << 8;

    return byte3 | byte2 | byte1 | byte0;
}

// sign-extend 8-bit to 32-bit
int32_t C_SignExt8to32(int32_t n)
{
    if (n & 0x00000080)
    {
        return 0xFFFFFF00 | n;
    }
    else
    {
        return n;
    }
}

// rotate right by 8 bit
uint32_t C_RotateRight8(uint32_t n)
{
    return (n >> 8) | (n << 24);
}

// multiply by 17 (without using multiply if in assembly)
int32_t C_Multiply17(int32_t x)
{
    return 17 * x;
}
