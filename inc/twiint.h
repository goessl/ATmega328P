/*
 * twiint.h
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



#ifndef TWIINT_H_
#define TWIINT_H_



#ifndef TWI_FREQUENCY
    #define TWI_FREQUENCY 400000
#endif

#define TWI_ADDRESS_W(x)    (((x) << 1) & ~0x01)
#define TWI_ADDRESS_R(x)    (((x) << 1) | 0x01)



#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>



void twiint_init(void);

bool twiint_busy(void);
void twiint_flush(void);

void twiint_start(uint8_t address, uint8_t *data, size_t len);



#endif /* TWIINT_H_ */