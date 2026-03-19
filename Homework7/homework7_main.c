/*
 * homework7_main.c – Full output with proper UART init
 * ECE 266, Spring 2026
 *
 * Requires uartstdio.c to be added to the project.
 */

#include <homework7.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

//============================================================================
// SysTick support (optional – remove if Util.lib is not used)
//============================================================================
volatile uint32_t EventTick = 0;

void Clock_InitSysTick(void)
{
    SysTickPeriodSet(16000);    // 1 ms tick (assuming 16 MHz PIOSC)
    SysTickIntEnable();
    SysTickEnable();
}

//============================================================================
// LED Blink – confirms program is running
//============================================================================
void BlinkLED(int times)
{
    int i;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) {}
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    for (i = 0; i < times; i++) {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); // green on
        SysCtlDelay(SysCtlClockGet() / 3 / 2);                 // 0.5 sec
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);          // off
        SysCtlDelay(SysCtlClockGet() / 3 / 2);
    }
}

//============================================================================
// Test functions – using UARTprintf
//============================================================================
void TestUmax()
{
    UARTprintf("\n\r*** Test umax functions ***\n\r");
    UARTprintf("umax_cb(15, 0) = %u\n\r", umax_cb(15, 0));
    UARTprintf("umax_cb(10, 20) = %u\n\r", umax_cb(10, 20));
    UARTprintf("umax_cb(0xFFFFFFFF, 1) = 0x%X\n\r", umax_cb(0xFFFFFFFF, 1));
    UARTprintf("umax_ce(15, 0) = %u\n\r", umax_ce(15, 0));
    UARTprintf("umax_ce(10, 20) = %u\n\r", umax_ce(10, 20));
    UARTprintf("umax_ce(0xFFFFFFFF, 1) = 0x%X\n\r", umax_ce(0xFFFFFFFF, 1));
}

void TestSign()
{
    UARTprintf("\n\r*** Test sign functions ***\n\r");
    UARTprintf("sign_cb(10) = %d\n\r", sign_cb(10));
    UARTprintf("sign_cb(-10) = %d\n\r", sign_cb(-10));
    UARTprintf("sign_cb(0) = %d\n\r", sign_cb(0));
    UARTprintf("sign_ce(10) = %d\n\r", sign_ce(10));
    UARTprintf("sign_ce(-10) = %d\n\r", sign_ce(-10));
    UARTprintf("sign_ce(0) = %d\n\r", sign_ce(0));
}

void TestSwap()
{
    unsigned short X[4];
    int flag;

    UARTprintf("\n\r*** Test swap functions ***\n\r");
    X[0] = 3; X[1] = 1; X[2] = 0xFFFF; X[3] = 4;
    UARTprintf("Test swap_cb\n\r");
    UARTprintf("1st swap, k = 0: [%u, %u, %u, %u] ",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3]);
    flag = swap_cb(X, 0);
    UARTprintf("=> [%u, %u, %u, %u], %s\n\r",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3],
            flag ? "swapped" : "not swapped");

    UARTprintf("2nd swap, k = 1: [%u, %u, %u, %u] ",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3]);
    flag = swap_cb(X, 1);
    UARTprintf("=> [%u, %u, %u, %u], %s\n\r",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3],
            flag ? "swapped" : "not swapped");

    UARTprintf("3rd swap, k = 2: [%u, %u, %u, %u] ",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3]);
    flag = swap_cb(X, 2);
    UARTprintf("=> [%u, %u, %u, %u], %s\n\r",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3],
            flag ? "swapped" : "not swapped");

    X[0] = 3; X[1] = 1; X[2] = 0xFFFF; X[3] = 4;
    UARTprintf("Test swap_ce\n\r");
    UARTprintf("1st swap, k = 0: [%u, %u, %u, %u] ",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3]);
    flag = swap_ce(X, 0);
    UARTprintf("=> [%u, %u, %u, %u], %s\n\r",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3],
            flag ? "swapped" : "not swapped");

    UARTprintf("2nd swap, k = 1: [%u, %u, %u, %u] ",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3]);
    flag = swap_ce(X, 1);
    UARTprintf("=> [%u, %u, %u, %u], %s\n\r",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3],
            flag ? "swapped" : "not swapped");

    UARTprintf("3rd swap, k = 2: [%u, %u, %u, %u] ",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3]);
    flag = swap_ce(X, 2);
    UARTprintf("=> [%u, %u, %u, %u], %s\n\r",
            (unsigned int) X[0], (unsigned int) X[1],
            (unsigned int) X[2], (unsigned int) X[3],
            flag ? "swapped" : "not swapped");
}

//============================================================================
// main
//============================================================================
int main(void)
{
    // Set system clock to 80 MHz (must be first)
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // --- UART Initialisation (critical for output) ---
    // Enable the UART and GPIO peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)) {}
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)) {}

    // Configure PA0 and PA1 as UART pins
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Set the UART clock source to the system clock (80 MHz)
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);

    // Initialise the UART stdio (baud rate 115200)
    UARTStdioConfig(0, 115200, SysCtlClockGet());

    // Optional: initialise SysTick (only if you still link Util.lib)
    // Clock_InitSysTick();

    // Blink LED to confirm we're alive
    BlinkLED(5);

    // Print header
    UARTprintf("\n\r*** Homework 7 - output follows ***\n\r");

    // Run tests
    TestUmax();
    TestSign();
    TestSwap();

    while (1) { }
}
