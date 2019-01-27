/*
 * UART.c
 * 
 * Author:      Sebastian Gössl
 * Hardware:    ATmega328P
 * 
 * LICENSE:
 * MIT License
 * 
 * Copyright (c) 2018 Sebastian Gössl
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */



#include <avr/io.h>
#include "UART.h"



#ifndef F_CPU
    #define F_CPU 16000000UL
    #warning "F_CPU not defined! Assuming 16MHz."
#endif



static int UART_putc(char c, FILE* stream)
{
    (void)stream;
    UART_transmit(c);
    return 0;
}
static int UART_getc(FILE* stream)
{
    (void)stream;
    return UART_receive();
}

static FILE UART_out = FDEV_SETUP_STREAM(UART_putc, NULL, _FDEV_SETUP_WRITE);
static FILE UART_in = FDEV_SETUP_STREAM(NULL, UART_getc, _FDEV_SETUP_READ);

FILE* UART_getOut(void)
{
    return &UART_out;
}
FILE* UART_getIn(void)
{
    return &UART_in;
}



bool UART_init(uint32_t baud, bool stdToUart)
{
    bool ret = UART_setBaud(baud);
    
    
    
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    
    if(stdToUart)
    {
        stdout = &UART_out;
        stdin = &UART_in;
    }
    
    
    return ret;
}

bool UART_setBaud(uint32_t baud)
{
    uint16_t ubrrValue = (F_CPU + 8*baud) / (16*baud) - 1;
    
    if(100 * F_CPU > 16 * (ubrrValue + 1) * (100 + BAUD_TOL) * baud
        || 100 * F_CPU < 16 * (ubrrValue + 1) * (100 - BAUD_TOL) * baud)
    {
        ubrrValue = (F_CPU + 4*baud) / (8*baud) - 1;
        UCSR0A |= (1 << U2X0);
    }
    
    UBRR0H = ubrrValue >> 8;
    UBRR0L = ubrrValue & 0xFF;
    
    return 100 * F_CPU > 8 * (ubrrValue + 1) * (100 + BAUD_TOL) * baud
        || 100 * F_CPU < 8 * (ubrrValue + 1) * (100 - BAUD_TOL) * baud;
}



static bool UART_isReceiveComplete(void)
{
    return UCSR0A & (1 << RXC0);
}

static bool UART_isTransmitComplete(void)
{
    return UCSR0A & (1 << TXC0);
}

static bool UART_isDataEmpty(void)
{
    return UCSR0A & (1 << UDRE0);
}



void UART_transmit(uint8_t data)
{
    while(!UART_isDataEmpty())
        ;
    
    UDR0 = data;
    
    while(!UART_isTransmitComplete())
        ;
}

void UART_transmitBurst(uint8_t* data, size_t len)
{
    while(len--)
    {
        while(!UART_isDataEmpty())
            ;
        
        UDR0 = *data++;
    }
    
    while(!UART_isTransmitComplete())
        ;
}


uint8_t UART_receive(void)
{
    while(!UART_isReceiveComplete())
        ;
    
    return UDR0;
}

void UART_receiveBurst(uint8_t* data, size_t len)
{
    while(len--)
        *data++ = UART_receive();
}
