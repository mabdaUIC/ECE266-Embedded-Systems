/*
 * homework6_main.c for homework 6
 * ECE 266, spring 2026
 * Created by Zhao Zhang
 * MODIFIED: Ultra‑safe character‑by‑character printing with delays
 */

#include <stdio.h>
#include "launchpad.h"
#include "homework6.h"
<<<<<<< HEAD
=======

// Very long delay to let UART transmit
void longDelay(void) {
    volatile int i;
    for (i = 0; i < 500000; i++);   // adjust if needed
}

// Print a string character by character with delays
void slowPrint(const char *s) {
    while (*s) {
        uprintf("%c", *s++);
        longDelay();
    }
}

// Print an integer as a string with delays
void slowPrintInt(int val) {
    char buffer[12];                 // enough for 32-bit int
    sprintf(buffer, "%d", val);
    slowPrint(buffer);
}

// Print a short as an integer
void slowPrintShort(short val) {
    slowPrintInt((int)val);
}

// Print the whole test line for copy functions
void printCopyLine(const char* label, int x[], int y[]) {
    slowPrint(label);
    slowPrint(" X1[] = {");
    slowPrintInt(x[0]); slowPrint(", ");
    slowPrintInt(x[1]); slowPrint(", ");
    slowPrintInt(x[2]); slowPrint(", ");
    slowPrintInt(x[3]); slowPrint("}, Y1[] = {");
    slowPrintInt(y[0]); slowPrint(", ");
    slowPrintInt(y[1]); slowPrint(", ");
    slowPrintInt(y[2]); slowPrint(", ");
    slowPrintInt(y[3]); slowPrint("}\n\r");
    longDelay(); longDelay();        // extra pause between lines
}

// Print swap test line
void printSwapLine(const char* label, short x[], short y[]) {
    slowPrint(label);
    slowPrint(" X2[] = {");
    slowPrintShort(x[0]); slowPrint(", ");
    slowPrintShort(x[1]); slowPrint(", ");
    slowPrintShort(x[2]); slowPrint(", ");
    slowPrintShort(x[3]); slowPrint(", ");
    slowPrintShort(x[4]); slowPrint("}, Y2[] = {");
    slowPrintShort(y[0]); slowPrint(", ");
    slowPrintShort(y[1]); slowPrint(", ");
    slowPrintShort(y[2]); slowPrint(", ");
    slowPrintShort(y[3]); slowPrint(", ");
    slowPrintShort(y[4]); slowPrint("}\n\r");
    longDelay(); longDelay();
}
>>>>>>> a84fa3336c2bcb2d3d96b4c53ae3efc1a299eea9

void ClearArray(int Y[])
{
    Y[0] = Y[1] = Y[2] = Y[3] = 0;
}

int main(void)
{
    int X1[4] = { 10, 20, 30, 40 };
    int Y1[4];
    short X2[5] = { 1, 2, 3, 4, 5 };
    short Y2[5] = { -1, -2, -3, -4, -5 };
    volatile int i;

    LaunchPadInit();

    // Very long initial delay to let UART stabilize
    for (i = 0; i < 5000000; i++);

    slowPrint("\n\r*** Test copyArray functions ***\n\r");
    longDelay();

    ClearArray(Y1);
    Copy4ByPreIndex(Y1, X1);
    printCopyLine("By pre-index:", X1, Y1);

    ClearArray(Y1);
    Copy4ByPreIndexUpdate(Y1, X1);
    printCopyLine("By pre-index with update:", X1, Y1);

    ClearArray(Y1);
    Copy4ByPostIndex(Y1, X1);
    printCopyLine("By post-index:", X1, Y1);

    ClearArray(Y1);
    Copy4ByRegOffset(Y1, X1);
    printCopyLine("By register offset:", X1, Y1);

    slowPrint("\n\r*** Test swap ***\n\r");
    longDelay();

    Swap2(X2, Y2, 1);
    printSwapLine("First swap, index 1:", X2, Y2);

    Swap2(X2, Y2, 3);
    printSwapLine("Second swap, index 3:", X2, Y2);

    // Never exit
    while (1) { }
}
