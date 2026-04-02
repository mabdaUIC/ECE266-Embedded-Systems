/*
 * homework8-main.c for homework 8
 * ECE 266, spring 2024
 * Created by Zhao Zhang
 */

#include <homework8.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"

// Test copy1()
void testCopy1()
{
    static short X[5] = { 10, -20, 30, -40, 50 };
    static short Y[5] = { 100, 100, 100, 100, 100 };

	uprintf("*** Test copy1() ***\n\r");
	uprintf("Before copying:\n\r");
	uprintf("X[] = {%d, %d, %d, %d, %d}\n\r", (int) X[0], (int) X[1],
			(int) X[2], (int) X[3], (int) X[4]);
	uprintf("Y[] = {%d, %d, %d, %d, %d}\n\r", (int) Y[0], (int) Y[1],
			(int) Y[2], (int) Y[3], (int) Y[4]);
	copy1(X, Y, 5);
	uprintf("After copying:\n\r");
	uprintf("X[] = {%d, %d, %d, %d, %d}\n\r", (int) X[0], (int) X[1],
			(int) X[2], (int) X[3], (int) X[4]);
	uprintf("Y[] = {%d, %d, %d, %d, %d}\n\r", (int) Y[0], (int) Y[1],
			(int) Y[2], (int) Y[3], (int) Y[4]);
}

void testSsat12Array()
{
    short X[] = {-10000, -20, 0, 20, 10000, -5000, -2048, 2047};

    uprintf("*** Test sat12Array function ***\n\r");
    uprintf("%d %d %d %d %d %d %d %d", (int) X[0], (int) X[1], (int) X[2], (int) X[3],
            (int) X[4], (int) X[5], (int) X[6], (int) X[7]);
    Ssat12Array(X, 8);
    uprintf(" => %d %d %d %d %d %d %d %d\n\r", (int) X[0], (int) X[1], (int) X[2], (int) X[3],
            (int) X[4], (int) X[5], (int) X[6], (int) X[7]);
}

// Test dotProduct()
void testDotProduct()
{
    short X[5] = {1, 2, 3, 4, -5};
    short Y[5] = {1, 2, 3, 4, 5};
    int result = DotProduct(X, Y, 5);

    uprintf("*** Test dotProduct function ***\n\r");
    uprintf("dotProduct([%d, %d, %d, %d, %d], [%d, %d, %d, %d, %d]) = %d\n\r",
            (int) X[0], (int) X[1], (int) X[2], (int) X[3], (int) X[4],
            (int) Y[0], (int) Y[1], (int) Y[2], (int) Y[3], (int) Y[4],
            result);
}

// Test countChar()
void testCountChar()
{
    char str[] = "This is homework 8";
    uprintf("*** Test countChar function ***\n\r");
    uprintf("count_char(\"%s\", '%c') = %d\n\r", str, '8', CountChar(str, '8'));
    uprintf("count_char(\"%s\", '%c') = %d\n\r", str, 'i', CountChar(str, 'i'));
    uprintf("count_char(\"%s\", '%c') = %d\n\r", str, 'j', CountChar(str, 'j'));
}

// The main() function
int main(void)
{
    LaunchPadInit();

    testCopy1();
    uprintf("\n\r");
    testSsat12Array();
    uprintf("\n\r");
    testDotProduct();
    uprintf("\n\r");
    testCountChar();
    uprintf("\n\r");

    // Never exit
    while (true) {
    }
}

