/*
 * seg7raw.c: Communication functions with the TiM1637 chip used with the
 * Groove 4-digit 7-segment display.
 *
 * Updated: Now uses PD0 (CLK) and PD1 (DIO) on Port D (J6 Grove connector).
 *
 * Created by Zhao Zhang for ECE 367 labs at UIC, fall 2016.
 * Modified for port D by F3 project, spring 2026.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include "launchpad.h"
#include "seg7.h"

// MCU connections to TiM1637's CLK and DIO pins.
// Grove J6 connector: PD0 = CLK, PD1 = DIO, 3.3V, GND
#define PORT_PERIPH     SYSCTL_PERIPH_GPIOD   // Enable Port D
#define PORT            GPIO_PORTD_BASE        // Base address of Port D
#define CLK             GPIO_PIN_0             // PD0 for clock
#define DIO             GPIO_PIN_1             // PD1 for data I/O

// Signal timing of CLK and DIO
#define HALF_CCT        5                       // Half clock cycle 5 us
#define DELTA           1                       // Time difference for tolerance

/*
 * Initialize the port connection to TiM1637 and the 7-segment display.
 * Uses PD0 and PD1 as GPIO (bit banging the TM1637 protocol).
 */
void Seg7Init()
{
    // Enable GPIO Port D
    SysCtlPeripheralEnable(PORT_PERIPH);

    // Set the pads for standard open-drain operation, with 2mA strength
    GPIOPadConfigSet(PORT, CLK | DIO, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD);

    // Make the pins outputs
    GPIODirModeSet(PORT, CLK | DIO, GPIO_DIR_MODE_OUT);

    // Initial states are high
    GPIOPinWrite(PORT, CLK | DIO, CLK | DIO);
}

// Send START bit: Keep CLK high, pull DIO from high to low.
static void tmSendStart()
{
    // Pull CLK high, wait delta, then set DIO high
    GPIOPinWrite(PORT, CLK, CLK);
    WaitUs(DELTA);
    GPIOPinWrite(PORT, DIO, DIO);
    WaitUs(HALF_CCT - DELTA);

    // Pull DIO low for half CCT
    GPIOPinWrite(PORT, DIO, 0);
    WaitUs(HALF_CCT);
}

static void tmSendDataBit(bool bit)
{
    uint8_t pinValue = bit ? DIO : 0;

    // CLK low, write data bit, wait half CCT minus delta
    GPIOPinWrite(PORT, CLK, 0);
    WaitUs(DELTA);
    GPIOPinWrite(PORT, DIO, pinValue);
    WaitUs(HALF_CCT - DELTA);

    // CLK high, wait half CCT
    GPIOPinWrite(PORT, CLK, CLK);
    WaitUs(HALF_CCT);
}

// Wait for ACK (does not check, just waits one CCT)
static void tmWaitAck()
{
    GPIOPinWrite(PORT, CLK, 0);
    GPIODirModeSet(PORT, DIO, GPIO_DIR_MODE_IN);   // release DIO for ACK

    WaitUs(HALF_CCT);
    GPIOPinWrite(PORT, CLK, CLK);
    WaitUs(HALF_CCT);

    GPIOPinWrite(PORT, CLK, 0);
    GPIODirModeSet(PORT, DIO, GPIO_DIR_MODE_OUT);   // back to output
}

// Send STOP bit: Both low, then DIO high in the middle
static void tmSendStop()
{
    // Pull CLK and DIO low
    GPIOPinWrite(PORT, CLK, 0);
    WaitUs(DELTA);
    GPIOPinWrite(PORT, DIO, 0);

    // Pull CLK high after half CCT
    WaitUs(HALF_CCT - DELTA);
    GPIOPinWrite(PORT, CLK, CLK);

    // Pull DIO high after another half CCT
    WaitUs(HALF_CCT);
    GPIOPinWrite(PORT, DIO, DIO);
}

// Send a single byte (8 bits, LSB first)
static void tmSendByte(uint8_t byte)
{
    int i;
    for (i = 0; i < 8; i++, byte >>= 1)
    {
        bool bit = (byte & 0b00000001);
        tmSendDataBit(bit);
    }
}

// Update all 4 digits of the 7-segment displays
void Seg7RawUpdate(uint8_t code[])
{
    tmSendStart();
    tmSendByte(0x40);       // data command, auto increment
    tmWaitAck();
    tmSendStop();

    tmSendStart();
    tmSendByte(0xc0);       // address command, start at digit 0
    tmWaitAck();
    tmSendByte(code[3]);    // leftmost digit in data order (TM1637 expects data[3] first)
    tmWaitAck();
    tmSendByte(code[2]);
    tmWaitAck();
    tmSendByte(code[1]);
    tmWaitAck();
    tmSendByte(code[0]);    // rightmost digit last
    tmWaitAck();
    tmSendStop();

    tmSendStart();
    tmSendByte(0x8A);       // display on, brightness 2/8
    tmWaitAck();
    tmSendStop();
}
