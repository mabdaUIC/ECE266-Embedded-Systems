/*
 * homework6_main.c for homework 6
 * ECE 266, spring 2026
 * Created by Zhao Zhang
 */

#include <stdio.h>
#include "launchpad.h"
#include "homework6.h"

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

    LaunchPadInit();

    //
    // Test copy functions
    //

    uprintf("\n\r*** Test copyArray functions ***\n\r");
    ClearArray(Y1);
    Copy4ByPreIndex(Y1, X1);
    uprintf("By pre-index: X1[] = {%d, %d, %d, %d}, Y1[] = {%d, %d, %d, %d}\n\r",
            X1[0], X1[1], X1[2], X1[3], Y1[0], Y1[1], Y1[2], Y1[3]);

    ClearArray(Y1);
    Copy4ByPreIndexUpdate(Y1, X1);
    uprintf("By pre-index with update: X1[] = {%d, %d, %d, %d}, Y1[] = {%d, %d, %d, %d}\n\r",
            X1[0], X1[1], X1[2], X1[3], Y1[0], Y1[1], Y1[2], Y1[3]);

    ClearArray(Y1);
    Copy4ByPostIndex(Y1, X1);
    uprintf("By post-index: X1[] = {%d, %d, %d, %d}, Y1[] = {%d, %d, %d, %d}\n\r",
            X1[0], X1[1], X1[2], X1[3], Y1[0], Y1[1], Y1[2], Y1[3]);

    ClearArray(Y1);
    Copy4ByRegOffset(Y1, X1);
    uprintf("By register offset: X1[] = {%d, %d, %d, %d}, Y1[] = {%d, %d, %d, %d}\n\r",
            X1[0], X1[1], X1[2], X1[3], Y1[0], Y1[1], Y1[2], Y1[3]);

    //
    // Test swap2 function
    //

    uprintf("\n\r*** Test swap ***\n\r");
    Swap2(X2, Y2, 1);
    uprintf("First swap, index 1: X2[] = {%d, %d, %d, %d, %d}, Y2[] = {%d, %d, %d, %d, %d}\n\r",
            (int) X2[0], (int) X2[1], (int) X2[2], (int) X2[3], (int) X2[4],
            (int) Y2[0], (int) Y2[1], (int) Y2[2], (int) Y2[3], (int) Y2[4]);

    Swap2(X2, Y2, 3);
    uprintf("Second swap, index 3: X2[] = {%d, %d, %d, %d, %d}, Y2[] = {%d, %d, %d, %d, %d}\n\r",
            (int) X2[0], (int) X2[1], (int) X2[2], (int) X2[3], (int) X2[4],
            (int) Y2[0], (int) Y2[1], (int) Y2[2], (int) Y2[3], (int) Y2[4]);

    // Never exit
    while (true)
    {
    }
}

