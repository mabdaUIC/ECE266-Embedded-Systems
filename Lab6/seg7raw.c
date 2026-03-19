/*
 * seg7raw.c – Low-level bit-banging for TiM1637.
 */
#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include "launchpad.h"
#include "seg7.h"

#define PORT_PERIPH     SYSCTL_PERIPH_GPIOA
#define PORT            GPIO_PORTA_BASE
#define CLK             GPIO_PIN_6
#define DIO             GPIO_PIN_7

#define HALF_CCT        5
#define DELTA           1

void Seg7Init()
{
    SysCtlPeripheralEnable(PORT_PERIPH);
    while(!SysCtlPeripheralReady(PORT_PERIPH));
    GPIOPadConfigSet(PORT, CLK | DIO, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD);
    GPIODirModeSet(PORT, CLK | DIO, GPIO_DIR_MODE_OUT);
    GPIOPinWrite(PORT, CLK | DIO, CLK | DIO);
}

static void tmSendStart()
{
    GPIOPinWrite(PORT, CLK, CLK);
    WaitUs(DELTA);
    GPIOPinWrite(PORT, DIO, DIO);
    WaitUs(HALF_CCT - DELTA);
    GPIOPinWrite(PORT, DIO, 0);
    WaitUs(HALF_CCT);
}

static void tmSendDataBit(bool bit)
{
    uint8_t pinValue = bit ? DIO : 0;
    GPIOPinWrite(PORT, CLK, 0);
    WaitUs(DELTA);
    GPIOPinWrite(PORT, DIO, pinValue);
    WaitUs(HALF_CCT - DELTA);
    GPIOPinWrite(PORT, CLK, CLK);
    WaitUs(HALF_CCT);
}

static void tmWaitAck()
{
    GPIOPinWrite(PORT, CLK, 0);
    GPIODirModeSet(PORT, DIO, GPIO_DIR_MODE_IN);
    WaitUs(HALF_CCT);
    GPIOPinWrite(PORT, CLK, CLK);
    WaitUs(HALF_CCT);
    GPIOPinWrite(PORT, CLK, 0);
    GPIODirModeSet(PORT, DIO, GPIO_DIR_MODE_OUT);
}

static void tmSendStop()
{
    GPIOPinWrite(PORT, CLK, 0);
    WaitUs(DELTA);
    GPIOPinWrite(PORT, DIO, 0);
    WaitUs(HALF_CCT - DELTA);
    GPIOPinWrite(PORT, CLK, CLK);
    WaitUs(HALF_CCT);
    GPIOPinWrite(PORT, DIO, DIO);
}

static void tmSendByte(uint8_t byte)
{
    int i;
    for (i = 0; i < 8; i++, byte >>= 1) {
        tmSendDataBit(byte & 0x01);
    }
}

void Seg7RawUpdate(uint8_t code[])
{
    tmSendStart();
    tmSendByte(0x40);
    tmWaitAck();
    tmSendStop();

    tmSendStart();
    tmSendByte(0xc0);
    tmWaitAck();
    tmSendByte(code[3]);
    tmWaitAck();
    tmSendByte(code[2]);
    tmWaitAck();
    tmSendByte(code[1]);
    tmWaitAck();
    tmSendByte(code[0]);
    tmWaitAck();
    tmSendStop();

    tmSendStart();
    tmSendByte(0x8A);
    tmWaitAck();
    tmSendStop();
}
