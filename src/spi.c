/*
 * spi.c
 * 
 * Minimalistic, blocking SPI driver.
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



#include <avr/io.h> //hardware registers
#include "spi.h"



//default to Arduino oscillator
#ifndef F_CPU
    #define F_CPU 16000000UL
    #warning "F_CPU not defined! Assuming 16MHz."
#endif


//try every prescaler so that the frequency will be
//between SPI_MIN_FREQUENCY and SPI_FREQUENCY
#if SPI_MIN_FREQUENCY <= F_CPU/2 && F_CPU/2 <= SPI_FREQUENCY
    #define SPI_PRESCALER 2
    #define SPR0_VALUE 0
    #define SPR1_VALUE 0
    #define SPI2X_VALUE 1
#elif SPI_MIN_FREQUENCY <= F_CPU/4 && F_CPU/4 <= SPI_FREQUENCY
    #define SPI_PRESCALER 4
    #define SPR0_VALUE 0
    #define SPR1_VALUE 0
    #define SPI2X_VALUE 0
#elif SPI_MIN_FREQUENCY <= F_CPU/8 && F_CPU/8 <= SPI_FREQUENCY
    #define SPI_PRESCALER 8
    #define SPR0_VALUE 1
    #define SPR1_VALUE 0
    #define SPI2X_VALUE 1
#elif SPI_MIN_FREQUENCY <= F_CPU/16 && F_CPU/16 <= SPI_FREQUENCY
    #define SPI_PRESCALER 16
    #define SPR0_VALUE 1
    #define SPR1_VALUE 0
    #define SPI2X_VALUE 0
#elif SPI_MIN_FREQUENCY <= F_CPU/32 && F_CPU/32 <= SPI_FREQUENCY
    #define SPI_PRESCALER 32
    #define SPR0_VALUE 0
    #define SPR1_VALUE 1
    #define SPI2X_VALUE 1
#elif SPI_MIN_FREQUENCY <= F_CPU/64 && F_CPU/64 <= SPI_FREQUENCY
    #define SPI_PRESCALER 64
    #define SPR0_VALUE 1
    #define SPR1_VALUE 1
    #define SPI2X_VALUE 1
#elif SPI_MIN_FREQUENCY <= F_CPU/128 && F_CPU/128 <= SPI_FREQUENCY
    #define SPI_PRESCALER 128
    #define SPR0_VALUE 1
    #define SPR1_VALUE 1
    #define SPI2X_VALUE 0
#else
    #error "No valid SPI_PRESCALER found!"
#endif

#if SPI_MODE == 0
    #define CPOL_VALUE 0
    #define CPHA_VALUE 0
#elif SPI_MODE == 1
    #define CPOL_VALUE 0
    #define CPHA_VALUE 1
#elif SPI_MODE == 2
    #define CPOL_VALUE 1
    #define CPHA_VALUE 0
#elif SPI_MODE == 3
    #define CPOL_VALUE 1
    #define CPHA_VALUE 1
#else
    #error "No valid SPI_MODE defined!"
#endif

#if SPI_DORD == 0
    #define DORD_VALUE 0
#elif SPI_DORD == 1
    #define DORD_VALUE 1
#else
    #error "No valid SPI_DORD defined!"
#endif



void spi_init(void)
{
    //SS, MOSI, SCK
    DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);
    //MISO
    //DDRB &= ~(1 << DDB4);
    
    
    SPSR |= (SPI2X_VALUE << SPI2X);
    SPCR |= (1 << SPE) | (DORD_VALUE << DORD) | (1 << MSTR)
        | (CPOL_VALUE << CPOL) | (CPHA_VALUE << CPHA)
        | (SPR1_VALUE << SPR1) | (SPR0_VALUE << SPR0);
}



uint8_t spi_writeRead(uint8_t data)
{
    SPDR = data;
    while(~SPSR & (1 << SPIF))
        ;
    
    return SPDR;
}

void spi_writeBurst(uint8_t *out, size_t len)
{
    while(len--)
        spi_writeRead(*out++);
}

void spi_readBurst(uint8_t *in, size_t len)
{
    while(len--)
        *in++ = spi_writeRead(0x00);
}

void spi_writeReadBurst(uint8_t *out, uint8_t *in, size_t len)
{
    while(len--)
        *in++ = spi_writeRead(*out++);
}
