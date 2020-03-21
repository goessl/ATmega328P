/*
 * SPI.h
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



#ifndef SPI_H_
#define SPI_H_



#include <stddef.h>
#include <stdint.h>



//Max frequency
#ifndef SPI_FREQUENCY
    #define SPI_FREQUENCY 1000000
#endif

#ifndef SPI_MIN_FREQUENCY
    #define SPI_MIN_FREQUENCY 0
#endif

#ifndef SPI_MODE
    #define SPI_MODE 0
#endif

#ifndef SPI_DORD
    #define SPI_DORD 0
#endif



void SPI_init(void);

uint8_t SPI_writeRead(uint8_t data);
void SPI_writeBurst(uint8_t* out, size_t len);
void SPI_readBurst(uint8_t* in, size_t len);
void SPI_writeReadBurst(uint8_t* out, uint8_t* in, size_t len);



#endif /* SPI_H_ */
