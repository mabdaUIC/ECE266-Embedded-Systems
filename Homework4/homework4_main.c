/*
 * homework4_main.c: Starter code for homework 4
 *
 * ECE 266, spring 2026
 * Created by Zhao Zhang
 */

#include <homework4.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"

// Test insertField()
void TestClearBits7to4()
{
    uprintf("\n\r*** Test ClearBits7to4() ***\n\r");
    uprintf("C_TestClearBits7to4(0x%08X) = 0x%08X\n\r", 0x33333333,
            C_ClearBits7to4(0x33333333));
    uprintf("  TestClearBits7to4(0x%08X) = 0x%08X\n\r", 0x33333333,
            ClearBits7to4(0x33333333));
}

// Test insertField()
void TestInsertField12Bits()
{
    uprintf("\n\r*** Test InsertField12Bits() ***\n\r");
    uprintf("C_InsertField12Bits(0x%08X, 0x%08X) = 0x%08X\n\r", 0x33333333,
            0x00000FFF, C_InsertField12Bits(0x33333333, 0x00000FFF));
    uprintf("  InsertField12Bits(0x%08X, 0x%08X) = 0x%08X\n\r", 0x33333333,
            0x00000FFF, InsertField12Bits(0x33333333, 0x00000FFF));
}

// Test reverseByte()
void TestReserveByte()
{
    uprintf("\n\r*** Test ReverseByte() ***\n\r");
    uprintf("C_ReverseByte(0x%X) = 0x%X\n\r", 0x89ABCDEF,
            C_ReverseByte(0x89ABCDEF));
    uprintf("  reverseByte(0x%X) = 0x%X\n\r", 0x89ABCDEF,
            ReverseByte(0x89ABCDEF));
}

// Test ssat10bits()
void TestSsat10Bits()
{
    uprintf("\n\r*** Test Ssat10bits() ***\n\r");
    uprintf("C_Ssat10bits(%d) = %d\n\r", -10000, C_Ssat10Bits(-10000));
    uprintf("  Ssat10bits(%d) = %d\n\r", -10000, Ssat10Bits(-10000));
    uprintf("C_Ssat10bits(0x%X) = 0x%X\n\r", -10000, C_Ssat10Bits(-10000));
    uprintf("  Ssat10bits(%X) = 0x%X\n\r", -10000, Ssat10Bits(-10000));
    uprintf("C_Ssat10bits(%d) = %d\n\r", -400, C_Ssat10Bits(-400));
    uprintf("  Ssat10bits(%d) = %d\n\r", -400, Ssat10Bits(-400));
    uprintf("C_Ssat10bits(%d) = %d\n\r", 400, C_Ssat10Bits(400));
    uprintf("  Ssat10bits(%d) = %d\n\r", 400, Ssat10Bits(400));
    uprintf("C_Ssat10bits(%d) = %d\n\r", 10000, C_Ssat10Bits(10000));
    uprintf("  Ssat10bits(%d) = %d\n\r", 10000, Ssat10Bits(10000));
}

// Test usat10bits()
void TestUsat10Bits()
{
    uprintf("\n\r*** Test Usat10bits() ***\n\r");
    uprintf("C_Ssat10bits(%d) = %d\n\r", 400, C_Usat10Bits(400));
    uprintf("  Ssat10bits(%d) = %d\n\r", 400, Usat10Bits(400));
    uprintf("C_Ssat10bits(%d) = %d\n\r", 10000, C_Usat10Bits(10000));
    uprintf("  Ssat10bits(%d) = %d\n\r", 10000, Usat10Bits(10000));
    uprintf("C_Ssat10bits(0x%X) = 0x%X\n\r", 10000, C_Usat10Bits(10000));
    uprintf("  Ssat10bits(0x%X) = 0x%X\n\r", 10000, Usat10Bits(10000));
}

// Test rev16ForByte()
void TestRev16ForByte()
{
    uprintf("\n\r*** Test Rev16ForByte() ***\n\r");
    uprintf("C_Rev16ForByte(0x%X) = 0x%X\n\r", 0x89ABCDEF,
            C_Rev16ForByte(0x89ABCDEF));
    uprintf("  Rev16ForByte(0x%X) = 0x%X\n\r", 0x89ABCDEF,
            Rev16ForByte(0x89ABCDEF));
}

// Test signExt8to32()
void TestSignExt8to32()
{
    uprintf("\n\r*** Test SignExt8to32() ***\n\r");
    uprintf("C_SignExt8to32(0x%X) = 0x%X\n\r", 0x80, C_SignExt8to32(0x80));
    uprintf("  SignExt8to32(0x%X) = 0x%X\n\r", 0x80, SignExt8to32(0x80));
}

// Test rotateRight8()
void TestRotateRight8()
{
    uprintf("\n\r*** Test RotateRight8() ***\n\r");
    uprintf("C_RotateRight8(0x%X) = 0x%X\n\r", 0x12345678,
            C_RotateRight8(0x12345678));
    uprintf("  RotateRight8(0x%X) = 0x%X\n\r", 0x12345678,
            RotateRight8(0x12345678));
}

// Test multiply17()
void TestMultiply17()
{
    uprintf("\n\r*** Test Multiply17() ***\n\r");
    uprintf("C_Multiply17(%d) = %d\n\r", -10, C_Multiply17(-10));
    uprintf("  Multiply17(%d) = %d\n\r", -10, Multiply17(-10));
}

int main(void)
{
    LaunchPadInit();

    TestClearBits7to4();
    TestInsertField12Bits();
    TestReserveByte();
    TestSsat10Bits();
    TestUsat10Bits();
    TestRev16ForByte();
    TestSignExt8to32();
    TestRotateRight8();
    TestMultiply17();

    // Never exit
    while (true)
    {
    }
}

