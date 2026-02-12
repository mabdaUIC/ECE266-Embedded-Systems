/*
 * homework3_main.c for homework 3
 * ECE 266, spring 2026
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "homework3.h"

// test Sub2()
void TestSub2()
{
    uprintf("\n\r*** Test Sub2() ***\n\r");
    uprintf("Sub2(%d, %d) = %d\n\r", 1, 2, Sub2(1, 2));
    uprintf("Sub2(%d, %d) = %d\n\r", 10, -5, Sub2(10, -5));
    uprintf("Sub2(%d, %d) = %d\n\r", -3, 0, Sub2(-3, 0));
}

// test Sub3()
void TestSub3()
{
    uprintf("\n\r*** Test Sub3() ***\n\r");
    uprintf("Sub3(%d, %d, %d) = %d\n\r", 1, 2, 3, Sub3(1, 2, 3));
    uprintf("Sub3(%d, %d, %d) = %d\n\r", -1, 2, 3, Sub3(-1, 2, 3));
    uprintf("Sub3(%d, %d, %d) = %d\n\r", 0, 10, -5, Sub3(0, 10, -5));
    uprintf("Sub3(%d, %d, %d) = %d\n\r", -5, -10, -15, Sub3(-5, -10, -15));
}

// test Calc()
void TestCalc()
{
    uprintf("\n\r*** Test Calc() ***\n\r");
    uprintf("Calc(%d, %d, %d) = %d\n\r", 1, 2, 3, Calc(1, 2, 3));
    uprintf("Calc(%d, %d, %d) = %d\n\r", -1, 2, 3, Calc(-1, 2, 3));
    uprintf("Calc(%d, %d, %d) = %d\n\r", 0, 10, -5, Calc(0, 10, -5));
    uprintf("Calc(%d, %d, %d) = %d\n\r", -5, -10, -15, Calc(-5, -10, -15));
}

// Test CallCalc()
void TestCallCalc()
{
    uprintf("\n\r*** Test callCalc() ***\n\r");
    uprintf("CallCalc() = %d\n\r", CallCalc());
}

// the main function
int main(void)
{
    LaunchPadInit();
    
    TestSub2();
    TestSub3();
    TestCalc();
    TestCallCalc();

    // Never exit
    while (true) {
    }
}

