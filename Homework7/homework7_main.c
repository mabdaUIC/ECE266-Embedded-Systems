/*
 * homework7_main.c for homework 7
 * ECE 266, spring 2026
 * Created by Zhao Zhang
 */

#include <homework7.h>
#include <stdio.h>
#include "launchpad.h"

//
// Test max functions
//
void TestUmax()
{
    uprintf("\n\r*** Test umax functions ***\n\r");
    uprintf("umax_cb(15, 0) = %u\n\r", umax_cb(15, 0));
    uprintf("umax_cb(10, 20) = %u\n\r", umax_cb(10, 20));
    uprintf("umax_cb(0xFFFFFFFF, 1) = 0x%X\n\r", umax_cb(0xFFFFFFFF, 1));

    uprintf("umax_ce(15, 0) = %u\n\r", umax_ce(15, 0));
    uprintf("umax_ce(10, 20) = %u\n\r", umax_ce(10, 20));
    uprintf("umax_ce(0xFFFFFFFF, 1) = 0x%X\n\r", umax_ce(0xFFFFFFFF, 1));
}


//
// Test sign functions
//
void TestSign()
{
    uprintf("\n\r*** Test sign functions ***\n\r");
    uprintf("sign_cb(10) = %d\n\r", sign_cb(10));
    uprintf("sign_cb(-10) = %d\n\r", sign_cb(-10));
    uprintf("sign_cb(0) = %d\n\r", sign_cb(0));

    uprintf("sign_ce(10) = %d\n\r", sign_ce(10));
    uprintf("sign_ce(-10) = %d\n\r", sign_ce(-10));
    uprintf("sign_ce(0) = %d\n\r", sign_ce(0));
}

//
// Test swap functions
//
void TestSwap()
{
    unsigned short X[4];
    int flag;

    uprintf("\n\r*** Test swap functions ***\n\r");
    X[0] = 3, X[1] = 1, X[2] = 0xFFFF, X[3] = 4;
    uprintf("Test swap_cb\n\r");
    uprintf("1st swap, k = 0: [%u, %u, %u, %u] ",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3]);
    flag = swap_cb(X, 0);
    uprintf("=> [%u, %u, %u, %u], %s\n\r",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3],
            flag ? "swapped" : "not swapped");
    uprintf("2nd swap, k = 1: [%u, %u, %u, %u] ",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3]);
    flag = swap_cb(X, 1);
    uprintf("=> [%u, %u, %u, %u], %s\n\r",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3],
            flag ? "swapped" : "not swapped");
    uprintf("3rd swap, k = 2: [%u, %u, %u, %u] ",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3]);
    flag = swap_cb(X, 2);
    uprintf("=> [%u, %u, %u, %u], %s\n\r",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3],
            flag ? "swapped" : "not swapped");

    uprintf("Test swap_ce\n\r");
    X[0] = 3, X[1] = 1, X[2] = 0xFFFF, X[3] = 4;
    uprintf("1st swap, k = 0: [%u, %u, %u, %u] ",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3]);
    flag = swap_ce(X, 0);
    uprintf("=> [%u, %u, %u, %u], %s\n\r",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3],
            flag ? "swapped" : "not swapped");
    uprintf("2nd swap, k = 1: [%u, %u, %u, %u] ",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3]);
    flag = swap_ce(X, 1);
    uprintf("=> [%u, %u, %u, %u], %s\n\r",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3],
            flag ? "swapped" : "not swapped");
    uprintf("3rd swap, k = 2: [%u, %u, %u, %u] ",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3]);
    flag = swap_ce(X, 2);
    uprintf("=> [%u, %u, %u, %u], %s\n\r",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3],
            flag ? "swapped" : "not swapped");
}

int main(void)
{
    LaunchPadInit();

    TestUmax();
    TestSign();
    TestSwap();

    // Never exit
    while (true)
    {
    }
}

