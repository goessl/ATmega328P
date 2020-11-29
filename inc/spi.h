/*
 * spi.h
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



#ifndef SPI_H_
#define SPI_H_



#include <stddef.h> //size_t type
#include <stdint.h> //uint8_t type



//default to 1MHz
#ifndef SPI_FREQUENCY
    #define SPI_FREQUENCY 1000000
#endif

//set lower limit if needed
#ifndef SPI_MIN_FREQUENCY
    #define SPI_MIN_FREQUENCY 0
#endif

//datasheet table 19-2
//(CPOL << 1) || (CPHA << 0)
#ifndef SPI_MODE
    #define SPI_MODE 0
#endif

//0: MSB first, 1: LSB first
#ifndef SPI_DORD
    #define SPI_DORD 0
#endif



/**
 * Initializes the SPI peripherals to operate at SPI_FREQUENCY
 * (maximum and optionally SPI_MIN_FREQUENCY minimum)
 * and in SPI_MODE and in SPI_DORD.
 * Configures SS, MOSI & SCK (PB2, PB3 & PB5) as outputs
 * and MISO (PB4) as input.
 * SS (PB2) has to be kept as output so that the SPI hardware
 * doesn't switch into slave mode.
 */
void spi_init(void);

/**
 * Shifts out a single byte while simultaneously shifting one in.
 * 
 * @param data byte to shift out
 * @return byte shifted in
 */
uint8_t spi_writeRead(uint8_t data);
/**
 * Shifts out multiple bytes, ignoring the MISO data.
 * 
 * @param out location of the bytes to shift out
 * @param len number of bytes to shift out
 */
void spi_writeBurst(uint8_t *out, size_t len);
/**
 * Shifts in multiple bytes while simultaneously shifting out zeros.
 * 
 * @param in location for the shifted in bytes to be written to
 * @param len number of bytes to shift in
 */
void spi_readBurst(uint8_t *in, size_t len);
/**
 * Shifts out multiple bytes while simultaneously shifting bytes in.
 * 
 * @param out location of the bytes to shift out
 * @param in location for the shifted in bytes to be written to
 * @param len number of bytes to shift out/in
 */
void spi_writeReadBurst(uint8_t *out, uint8_t *in, size_t len);



#endif /* SPI_H_ */
