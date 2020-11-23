/*
 * uartint.h
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



#ifndef UARTINT_H_
#define UARTINT_H_



#ifndef BAUD
    #define BAUD 9600
#endif

#ifndef UARTINT_BUF_LEN
    #define UARTINT_BUF_LEN 80
#endif



#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>



extern FILE uartint_out;
extern FILE uartint_in;



void uartint_init(void);

char *uartint_ngets(char *s, size_t len);

size_t uartint_transmitAvailable(void);
void uartint_transmitFlush(void);
bool uartint_transmit(uint8_t data);
size_t uartint_transmitBurst(uint8_t *data, size_t len);

size_t uartint_receiveAvailable(void);
bool uartint_receivePeek(uint8_t *data);
bool uartint_receive(uint8_t *data);
size_t uartint_receiveBurst(uint8_t *data, size_t len);



#endif /* UARTINT_H_ */
