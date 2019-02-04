/*
 * UART2.h
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



#ifndef UART2_H_
#define UART2_H_



#ifndef BAUD
    #define BAUD 9600
#endif

#ifndef UART2_BUF_LEN
    #define UART2_BUF_LEN 80
#endif

#define UART2_OVERWRITE
#define UART2_STD



#include <stddef.h>
#include <stdint.h>
#include <stdio.h>



extern FILE UART2_out;
extern FILE UART2_in;



void UART2_init(void);

size_t UART2_transmitAvailable(void);
void UART2_transmitFlush(void);
int UART2_transmit(uint8_t data);
int UART2_transmitBurst(uint8_t* data, size_t len);

size_t UART2_receiveAvailable(void);
int UART2_receivePeek(uint8_t* data);
int UART2_receive(uint8_t* data);
int UART2_receiveBurst(uint8_t* data, size_t len);



#endif /* UART2_H_ */
