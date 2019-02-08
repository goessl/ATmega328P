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


#define UART_UBRR_TO_BAUD(ubrr)     (F_CPU / (16 * ((uint32_t)(ubrr) + 1)))
#define UART_UBRR_TO_BAUD_2X(ubrr)  (F_CPU / (8 * ((uint32_t)(ubrr) + 1)))

#define UART_BAUD_TO_UBRR(baud)     (F_CPU / (16 * (baud)) - 1)
#define UART_BAUD_TO_UBRR_2X(baud)  (F_CPU / (8 * (baud)) - 1)


#define UART_BAUD_MIN_N2X   UART_UBRR_TO_BAUD(UINT16_MAX)
#define UART_BAUD_MAX_N2X   UART_UBRR_TO_BAUD(0)
#define UART_BAUD_MIN_W2X   UART_UBRR_TO_BAUD_2X(UINT16_MAX)
#define UART_BAUD_MAX_W2X   UART_UBRR_TO_BAUD_2X(0)



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

FILE UART_out = FDEV_SETUP_STREAM(UART_putc, NULL, _FDEV_SETUP_WRITE);
FILE UART_in = FDEV_SETUP_STREAM(NULL, UART_getc, _FDEV_SETUP_READ);



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
    bool use2x;
    uint16_t ubrr;
    uint32_t baudReal;
    
    
    
    if(baud < UART_BAUD_MIN_W2X) //Only possible without use2x
    {
        use2x = false;
        ubrr = UART_BAUD_TO_UBRR(baud);
        baudReal = UART_UBRR_TO_BAUD(ubrr);
    }
    else
    {
        use2x = true;
        ubrr = UART_BAUD_TO_UBRR_2X(baud);
        baudReal = UART_UBRR_TO_BAUD_2X(ubrr);
    }
    
    if(use2x)
        UCSR0A |= (1 << U2X0);
    else
        UCSR0A &= ~(1 << U2X0);
    UBRR0 = ubrr;
    
    
    //Check error: (int32_t)(100 * baudReal / baud) - 100 > UART_BAUD_TOL
    return 100 * baudReal > baud * (100 + UART_BAUD_TOL)
        || 100 * baudReal < baud * (100 - UART_BAUD_TOL);
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
