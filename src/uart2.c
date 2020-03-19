/*
 * uart2.c
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
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "ring.h"
#include "uart2.h"



#ifndef F_CPU
    #define F_CPU 16000000UL
    #warning "F_CPU not defined! Assuming 16MHz."
#endif



#include <util/setbaud.h>



static int uart2_putc(char c, FILE* stream)
{
    (void)stream;
    if(uart2_transmit(c))
        return _FDEV_EOF;
    
    return c;
}

static int uart2_getc(FILE* stream)
{
    char c;
    
    (void)stream;
    if(uart2_receive((uint8_t*)&c))
        return _FDEV_EOF;
    
    return c;
}

FILE uart2_out = FDEV_SETUP_STREAM(uart2_putc, NULL, _FDEV_SETUP_WRITE);
FILE uart2_in = FDEV_SETUP_STREAM(NULL, uart2_getc, _FDEV_SETUP_READ);



static volatile uint8_t uart2_transmitArray[UART2_BUF_LEN],
    uart2_receiveArray[UART2_BUF_LEN];
static volatile ring_t uart2_transmitBuf, uart2_receiveBuf;



void uart2_init(void)
{
    uart2_transmitBuf =
        ring_init((uint8_t*)uart2_transmitArray, UART2_BUF_LEN);
    uart2_receiveBuf =
        ring_init((uint8_t*)uart2_receiveArray, UART2_BUF_LEN);
    
    
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    #if USE_2X
        UCSR0A |= (1 << U2X0);
    #endif
    
    UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
    
    #ifndef NO_UART_STD
        stdout = &uart2_out;
        stdin = &uart2_in;
    #endif
}



size_t uart2_transmitAvailable(void)
{
    size_t ret;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ret = ring_pushAvailable(uart2_transmitBuf);
    }
    
    return ret;
}

void uart2_transmitFlush(void)
{
    bool empty;
    
    do
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            empty = ring_isEmpty(uart2_transmitBuf);
        }
    } while(!empty);
}

bool uart2_transmit(uint8_t data)
{
    bool fail;
    
    
    while(uart2_transmitAvailable() < 1)
        ;
    
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        fail = ring_push((ring_t*)&uart2_transmitBuf, data);
    }
    
    if(!fail)
        UCSR0B |= (1 << UDRIE0);
    
    return fail;
}

size_t uart2_transmitBurst(uint8_t* data, size_t len)
{
    size_t i = 0;
    
    while(i<len && !uart2_transmit(*data++))
        i++;
    
    return i;
}


size_t uart2_receiveAvailable(void)
{
    size_t ret;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ret = ring_popAvailable(uart2_receiveBuf);
    }
    
    return ret;
}

bool uart2_receivePeek(uint8_t* data)
{
    bool ret;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ret = ring_peek((ring_t*)&uart2_receiveBuf, data);
    }
    
    return ret;
}

bool uart2_receive(uint8_t* data)
{
    bool ret;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ret = ring_pop((ring_t*)&uart2_receiveBuf, data);
    }
    
    return ret;
}

size_t uart2_receiveBurst(uint8_t* data, size_t len)
{
    size_t i = 0;
    
    while(i<len && !uart2_receive(data++))
        i++;
    
    return i;
}



ISR(USART_UDRE_vect)
{
    uint8_t c;
    
    if(!ring_pop((ring_t*)&uart2_transmitBuf, &c))
        UDR0 = c;
    else
        UCSR0B &= ~(1 << UDRIE0);
}

ISR(USART_RX_vect)
{
    #ifdef UART2_OVERWRITE
        ring_pushOver((ring_t*)&uart2_receiveBuf, UDR0);
    #else
        ring_push((ring_t*)&uart2_receiveBuf, UDR0);
    #endif
}
