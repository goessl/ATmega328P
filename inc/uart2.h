/*
 * uart2.h
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



#ifndef UART2_H_
#define UART2_H_



#ifndef BAUD
    #define BAUD 9600
#endif

#ifndef UART2_BUF_LEN
    #define UART2_BUF_LEN 80
#endif



#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>



extern FILE uart2_out;
extern FILE uart2_in;



void uart2_init(void);

size_t uart2_transmitAvailable(void);
void uart2_transmitFlush(void);
bool uart2_transmit(uint8_t data);
size_t uart2_transmitBurst(uint8_t* data, size_t len);

size_t uart2_receiveAvailable(void);
bool uart2_receivePeek(uint8_t* data);
bool uart2_receive(uint8_t* data);
size_t uart2_receiveBurst(uint8_t* data, size_t len);



#endif /* UART2_H_ */
