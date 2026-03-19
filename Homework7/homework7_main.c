/*
 * homework7_main.c – Homework 7 test harness
 * ECE 266, Spring 2026
 *
 * Modified to include LED blink and robust UART init.
 * (Only homework7_asmfunc.asm needs to be submitted.)
 */

#include <homework7.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "launchpad.h"

//============================================================================
// SysTick support – required by Util.lib
//============================================================================
volatile uint32_t EventTick = 0;   // DO NOT remove – used by Util

// DO NOT define SysTick_Handler – Util.lib already provides it.

// Initialize SysTick for a 1 ms timebase
void Clock_InitSysTick(void)
{
    // System clock is 16 MHz on the LaunchPad
    SysTickPeriodSet(16000);    // 16 MHz / 16000 = 1000 Hz → 1 ms period
    SysTickIntEnable();
    SysTickEnable();
}

//============================================================================
// LED blink test – confirms program is running even without UART
//============================================================================
void BlinkLED(int times)
{
    // Enable the GPIO port for the on‑board LED (Port F)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) {}

    // Set PF1 (red), PF2 (blue), PF3 (green) as outputs
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    int i;
    for (i = 0; i < times; i++) {
        // Green on
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
        SysCtlDelay(16000000 / 3 / 2);   // ~0.5 sec (16 MHz / 3 cycles per loop)
        // Green off
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
        SysCtlDelay(16000000 / 3 / 2);
    }
}

//============================================================================
// Test functions (identical to original)
//============================================================================
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

//============================================================================
// main
//============================================================================
int main(void)
{
    // Step 1: Provide timebase for Util.lib
    Clock_InitSysTick();

    // Step 2: Initialize Util (UART, buttons, etc.)
    LaunchPadInit();

    // Step 3: Blink LED to confirm program is running
    BlinkLED(5);   // Green LED flashes 5 times

    // Step 4: Send first UART message
    uprintf("\n\r*** Homework 7 – output follows ***\n\r");

    // Step 5: Run tests
    TestUmax();
    TestSign();
    TestSwap();

    while (true)
    {
        // Optionally blink slowly to indicate idle
        // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); // blue on
        // SysCtlDelay(16000000 / 3);   // 1 sec
        // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
        // SysCtlDelay(16000000 / 3);
    }
}
