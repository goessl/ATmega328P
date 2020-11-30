/*
 * uart.h
 * 
 * Minimalistic, blocking UART driver.
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



#ifndef UART_H_
#define UART_H_



#include <stddef.h> //size_t type
#include <stdint.h> //uint8_t type
#include <stdio.h>  //FILE type



//default to 9600
#ifndef BAUD
    #define BAUD 9600
#endif



/** Stream that outputs to the UART. */
extern FILE uart_out;
/** Stream that reads from the UART. */
extern FILE uart_in;



/**
 * Initializes the UART peripherals to enable transmitting and receiving
 * and redirects stdout and stdin to the UART if NO_UART_STD is not defined.
 * The BAUD rate is set by using setbaud.h.
 */
void uart_init(void);

/**
 * Transmits a single byte and blocks until the transmission is completed.
 * 
 * @param data byte to transmit
 */
void uart_transmit(uint8_t data);
/**
 * Transmits the bytes from the given location
 * and blocks until the transmission is completed.
 * 
 * @param data location of the bytes to transmit
 * @param len number of bytes to transmit
 */
void uart_transmitBurst(uint8_t *data, size_t len);

/**
 * Blocks until a single byte is received and then returns it.
 * 
 * @return the received byte
 */
uint8_t uart_receive(void);
/**
 * Blocks until the given number of bytes are received
 * and writes them to the provided location.
 * 
 * @param data location for the received bytes to be written to
 * @param len number of bytes to receive
 */
void uart_receiveBurst(uint8_t *data, size_t len);



#endif /* UART_H_ */
