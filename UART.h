/*
 * UART.h
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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */



#ifndef UART_H_
#define UART_H_



#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>



#ifndef UART_BAUD_TOL
    #define UART_BAUD_TOL 2
#endif

#define UART_BAUD_MIN   ((uint32_t)(F_CPU / (16 * ((uint32_t)UINT16_MAX + 1))))
#define UART_BAUD_MAX   ((uint32_t)(F_CPU / (8 * (0 + 1))))


extern FILE UART_out;
extern FILE UART_in;



bool UART_init(uint32_t baud, bool stdToUart);
bool UART_setBaud(uint32_t baud);

void UART_transmit(uint8_t data);
void UART_transmitBurst(uint8_t* data, size_t len);

uint8_t UART_receive(void);
void UART_receiveBurst(uint8_t* data, size_t len);



#endif /* UART_H_ */
