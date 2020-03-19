/*
 * uart.c
 * 
 * Author:      Sebastian Gössl
 * Hardware:    ATmega328P
 * 
 * LICENSE:
 * MIT License
 * 
 * Copyright (c) 2018 Sebastian Gössl
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */



#include <avr/io.h>
#include <stdbool.h>
#include "uart.h"



#ifndef F_CPU
    #define F_CPU 16000000UL
    #warning "F_CPU not defined! Assuming 16MHz."
#endif



#include <util/setbaud.h>



static int uart_putc(char c, FILE* stream)
{
    (void)stream;
    uart_transmit(c);
    return 0;
}

static int uart_getc(FILE* stream)
{
    (void)stream;
    return uart_receive();
}

FILE uart_out = FDEV_SETUP_STREAM(uart_putc, NULL, _FDEV_SETUP_WRITE);
FILE uart_in = FDEV_SETUP_STREAM(NULL, uart_getc, _FDEV_SETUP_READ);



void uart_init(void)
{
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    
    #if USE_2X
        UCSR0A |= (1 << U2X0);
    #endif
    
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    
    #ifndef NO_UART_STD
        stdout = &uart_out;
        stdin = &uart_in;
    #endif
}



static bool uart_isReceiveComplete(void)
{
    return UCSR0A & (1 << RXC0);
}

static bool uart_isTransmitComplete(void)
{
    return UCSR0A & (1 << TXC0);
}

static bool uart_isDataEmpty(void)
{
    return UCSR0A & (1 << UDRE0);
}



void uart_transmit(uint8_t data)
{
    while(!uart_isDataEmpty())
        ;
    
    UDR0 = data;
    
    while(!uart_isTransmitComplete())
        ;
}

void uart_transmitBurst(uint8_t* data, size_t len)
{
    while(len--)
    {
        while(!uart_isDataEmpty())
            ;
        
        UDR0 = *data++;
    }
    
    while(!uart_isTransmitComplete())
        ;
}


uint8_t uart_receive(void)
{
    while(!uart_isReceiveComplete())
        ;
    
    return UDR0;
}

void uart_receiveBurst(uint8_t* data, size_t len)
{
    while(len--)
        *data++ = uart_receive();
}
