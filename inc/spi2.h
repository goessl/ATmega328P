/*
 * spi2.h
 * 
 * Author:      Sebastian Gössl
 * Hardware:    ATmega328P
 * 
 * LICENSE:
 * MIT License
 * 
 * Copyright (c) 2019 Sebastian Gössl
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



#ifndef SPI2_H_
#define SPI2_H_



#include <stdbool.h>
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



void spi2_init(void);

bool spi2_isBusy(void);
void spi2_flush(void);

void spi2_transmitBurst(uint8_t* out, uint8_t* in, size_t len,
    uint8_t* port, uint8_t pin);



#endif /* SPI2_H_ */
