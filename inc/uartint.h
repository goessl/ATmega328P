/*
 * uartint.h
 * 
 * Buffered, interrupt based UART driver.
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



#include <stdbool.h>    //bool type
#include <stddef.h>     //size_t type, NULL pointer
#include <stdint.h>     //uint8_t type
#include <stdio.h>      //FILE type



//default to 9600
#ifndef BAUD
    #define BAUD 9600
#endif

//default to 80 (standard number of columns in the terminal)
#ifndef UARTINT_BUF_LEN
    #define UARTINT_BUF_LEN 80
#endif



/** Stream that outputs to the UART. */
extern FILE uartint_out;
/** Stream that reads from the UART. */
extern FILE uartint_in;



/**
 * Initializes the UART peripherals to enable transmitting and receiving
 * and redirects stdout and stdin to the UART if NO_UART_STD is not defined.
 * Interrupts have to be enabled (by calling avr/interrupt.h's sei).
 * On default, the receiver fifo rejects new data when it overflows
 * but by defining UARTINT_OVERWRITE old data will be overwritten on overflow.
 * The BAUD rate is set by using setbaud.h.
 */
void uartint_init(void);

/**
 * Reads available bytes from the receive buffer to the provided location
 * and returns the location when a complete line (\n terminated)
 * or (len-1) characters have been received
 * (the string will then be \0 terminated) or NULL otherwise.
 * The line terminator will be included in the finished string
 * so the user can find out if the line was terminated
 * or the character limit was reached (no \n at the end).
 * This functions keeps track of its current position in the string and
 * is meant to be called multiple times
 * until the line has completely been received.
 * For example:
 * 
 * char s[80];
 * while(1)
 *     if(uartint_ngets(s, 80))
 *         doSomethingWithLine(s);
 * 
 * https://gist.github.com/sebig3000/17c049f3562fccbbdfaeff090d166d60
 * 
 * @param s string is is beeing built
 * @param len maximum number of characters to read (including \0 terminator)
 * @return s when a line was completed
 * (line terminator or character limit reached)
 * or NULL is there are no more characters in the receive buffer
 */
char *uartint_ngets(char *s, size_t len);

/**
 * Returns the number of free bytes in the transmit buffer.
 * 
 * @return the number of free bytes in the transmit buffer
 */
size_t uartint_transmitAvailable(void);
/**
 * Blocks until all bytes in the transmit buffer have been transmitted.
 */
void uartint_transmitFlush(void);
/**
 * Adds a byte to the transmit buffer.
 * If there are no free locations in the transmit buffer,
 * this function blocks until it can add the byte to the buffer.
 * 
 * @param data byte to add to the transmit buffer
 * @return 0 if the byte was successfully added to the buffer, 1 otherwise
 */
bool uartint_transmit(uint8_t data);
/**
 * Adds multiple bytes to the transmit buffer.
 * If there are not enough locations in the transmit buffer,
 * this function blocks until it can add all bytes to the buffer.
 * 
 * @param data location of the bytes that should be added to the buffer
 * @return number of bytes that have successfully been added to the buffer
 * (len on success)
 */
size_t uartint_transmitBurst(uint8_t *data, size_t len);

/**
 * Returns the number of bytes in the receive buffer available to be read.
 */
size_t uartint_receiveAvailable(void);
/**
 * Reads a single byte from the receive buffer without removing it
 * and writes it to the provided location.
 * 
 * @return 0 on success, otherwise 1 (no bytes available)
 */
bool uartint_receivePeek(uint8_t *data);
/**
 * Removes a single byte from the receive buffer
 * and writes it to the provided location.
 * 
 * @return 0 on success, otherwise 1 (no bytes available)
 */
bool uartint_receive(uint8_t *data);
/**
 * Removes up to (len) bytes from the receive buffer
 * and writes them to the provided location.
 * Stops when either (len) bytes have have been read
 * or there are no more bytes in the receive buffer.
 * 
 * @param data location for the received bytes to be written to
 * @param len number of received bytes to read
 * @return the number of bytes that have been read (len on success)
 */
size_t uartint_receiveBurst(uint8_t *data, size_t len);



#endif /* UARTINT_H_ */
