/*
 * uartint.c
 * 
 * Author:      Sebastian Goessl
 * Hardware:    ATmega328P
 * 
 * LICENSE:
 * MIT License
 * 
 * Copyright (c) 2018 Sebastian Goessl
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
#include "uartint.h"



#ifndef F_CPU
    #define F_CPU 16000000UL
    #warning "F_CPU not defined! Assuming 16MHz."
#endif



#include <util/setbaud.h>



static int uartint_putc(char c, FILE *stream)
{
    (void)stream;
    if(uartint_transmit(c))
        return _FDEV_EOF;
    
    return c;
}

static int uartint_getc(FILE *stream)
{
    uint8_t c;
    
    (void)stream;
    if(uartint_receive(&c))
        return _FDEV_EOF;
    
    return c;
}

FILE uartint_out = FDEV_SETUP_STREAM(uartint_putc, NULL, _FDEV_SETUP_WRITE);
FILE uartint_in = FDEV_SETUP_STREAM(NULL, uartint_getc, _FDEV_SETUP_READ);



static volatile Ring_t uartint_transmitBuf, uartint_receiveBuf;
static volatile uint8_t uartint_transmitArray[UARTINT_BUF_LEN],
    uartint_receiveArray[UARTINT_BUF_LEN];



void uartint_init(void)
{
    uartint_transmitBuf =
        ring_init((uint8_t*)uartint_transmitArray, UARTINT_BUF_LEN);
    uartint_receiveBuf =
        ring_init((uint8_t*)uartint_receiveArray, UARTINT_BUF_LEN);
    
    
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    #if USE_2X
        UCSR0A |= (1 << U2X0);
    #endif
    
    UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
    
    #ifndef NO_UART_STD
        stdout = &uartint_out;
        stdin = &uartint_in;
    #endif
}



//Non-blocking gets, returns s when the line is complete
//https://gist.github.com/sebig3000/17c049f3562fccbbdfaeff090d166d60
char *uartint_ngets(char *s, size_t n)
{
    uint8_t c;
    static size_t i = 0;
    
    
    while(!uartint_receive(&c))
    {
        s[i++] = c;
        
        if(c == '\n' || i >= n-1)
        {
            s[i] = '\0';
            i = 0;
            
            return s;
        }
    }
    
    return NULL;
}



size_t uartint_transmitAvailable(void)
{
    size_t ret;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ret = ring_pushAvailable(uartint_transmitBuf);
    }
    
    return ret;
}

void uartint_transmitFlush(void)
{
    bool empty;
    
    do
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            empty = ring_isEmpty(uartint_transmitBuf);
        }
    } while(!empty);
}

bool uartint_transmit(uint8_t data)
{
    bool fail;
    
    
    while(uartint_transmitAvailable() < 1)
        ;
    
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        fail = ring_push((Ring_t*)&uartint_transmitBuf, data);
    }
    
    if(!fail)
        UCSR0B |= (1 << UDRIE0);
    
    return fail;
}

size_t uartint_transmitBurst(uint8_t *data, size_t len)
{
    size_t i = 0;
    
    while(i<len && !uartint_transmit(*data++))
        i++;
    
    return i;
}


size_t uartint_receiveAvailable(void)
{
    size_t ret;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ret = ring_popAvailable(uartint_receiveBuf);
    }
    
    return ret;
}

bool uartint_receivePeek(uint8_t *data)
{
    bool ret;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ret = ring_peek((Ring_t*)&uartint_receiveBuf, data);
    }
    
    return ret;
}

bool uartint_receive(uint8_t* data)
{
    bool ret;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ret = ring_pop((Ring_t*)&uartint_receiveBuf, data);
    }
    
    return ret;
}

size_t uartint_receiveBurst(uint8_t* data, size_t len)
{
    size_t i = 0;
    
    while(i<len && !uartint_receive(data++))
        i++;
    
    return i;
}



ISR(USART_UDRE_vect)
{
    uint8_t c;
    
    if(!ring_pop((Ring_t*)&uartint_transmitBuf, &c))
        UDR0 = c;
    else
        UCSR0B &= ~(1 << UDRIE0);
}

ISR(USART_RX_vect)
{
    #ifdef UARTINT_OVERWRITE
        ring_pushOver((Ring_t*)&uartint_receiveBuf, UDR0);
    #else
        ring_push((Ring_t*)&uartint_receiveBuf, UDR0);
    #endif
}
