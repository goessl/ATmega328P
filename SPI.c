/*
 * SPI.c
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
#include "SPI.h"



#ifndef F_CPU
    #define F_CPU 16000000UL
    #warning "F_CPU not defined! Assuming 16MHz."
#endif



void SPI_init(uint32_t frequency, bool lsbFirst, bool cpol, bool cpha)
{
    bool spi2xValue = false;
    uint8_t sprValue = 0;
    
    
    
    DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);
    //DDRB &= ~(1 << DDB4);
    
    
    if(F_CPU / 2 <= frequency) {
        spi2xValue = true;
        sprValue = 0;
    } else if(F_CPU / 4 <= frequency) {
        spi2xValue = false;
        sprValue = 0;
    } else if(F_CPU / 8 <= frequency) {
        spi2xValue = true;
        sprValue = (1 << SPR0);
    } else if(F_CPU / 16 <= frequency) {
        spi2xValue = false;
        sprValue = (1 << SPR0);
    } else if(F_CPU / 32 <= frequency) {
        spi2xValue = true;
        sprValue = (1 << SPR1);
    } else if(F_CPU / 64 <= frequency) {
        spi2xValue = false;
        sprValue = (1 << SPR1);
    } else if(F_CPU / 128 <= frequency) {
        spi2xValue = false;
        sprValue = (1 << SPR1) | (1 << SPR0);
    }
    
    if(spi2xValue)
        SPSR |= (1 << SPI2X);
    SPCR |= (1 << SPE) | ((lsbFirst?1:0) << DORD) | (1 << MSTR)
        | ((cpol?1:0) << CPOL) | ((cpha?1:0) << CPHA) | sprValue;
}


uint8_t SPI_transmit(uint8_t data)
{
    SPDR = data;
    while(~SPSR & (1 << SPIF))
        ;
    
    return SPDR;
}

void SPI_transmitBurst(uint8_t* out, uint8_t* in, size_t len)
{
    while(len--)
        *in++ = SPI_transmit(*out++);
}
