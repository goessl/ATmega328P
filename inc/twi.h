/*
 * twi.h
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



#ifndef TWI_H_
#define TWI_H_



#ifndef TWI_FREQUENCY
    //Default to Fast Mode
    #define TWI_FREQUENCY 400000
#endif



#include <stdbool.h>    //bool
#include <stdint.h>     //fixed width integers
#include <stddef.h>     //size_t



/**
 * Initializes the TWI hardware for master mode.
 */
void twi_init(void);

/**
 * Sends a START condition.
 * Returns 0 on success.
 */
bool twi_start(void);
/**
 * Sends a repeated START condition.
 * Returns 0 on success.
 */
bool twi_repStart(void);
/**
 * Sends a STOP condition.
 */
void twi_stop(void);

/**
 * Sends the slave address with intend to write.
 * Returns 0 if an acknowledge bit was returned.
 */
bool twi_addressWrite(uint8_t address);
/**
 * Sends the slave address with intend to read.
 * Returns 0 if an acknowledge bit was returned.
 */
bool twi_addressRead(uint8_t address);

/**
 * Sends a data byte.
 * Returns 0 if an acknowledge bit was returned.
 */
bool twi_write(uint8_t data);
/**
 * Sends data bytes until all bytes have been sent or no acknowledge bit has been returned.
 * Returns the number of bytes for which an acknowledge bit has been returned (len-1 on success).
 */
size_t twi_writeBurst(uint8_t *data, size_t len);

/**
 * Reads a data byte and sends an acknowledge bit.
 * Returns 0 on success.
 */
bool twi_readAck(uint8_t *data);
/**
 * Reads data bytes and returns acknowledge bits until all bytes have been read or a read has failed.
 * Returns the number of bytes that have been read successfully.
 */
size_t twi_readAckBurst(uint8_t *data, size_t len);
/**
 * Reads a data byte and sends no acknowledge bit.
 * Returns 0 on success.
 */
bool twi_readNoAck(uint8_t *data);
/**
 * Reads data bytes and returns no acknowledge bits until all bytes have been read or a read has failed.
 * Returns the number of bytes that have been read successfully.
 */
size_t twi_readNoAckBurst(uint8_t *data, size_t len);

/**
 * Writes the data to the given slave.
 */
bool twi_writeToSlave(uint8_t address, uint8_t *data, size_t len);
/**
 * Reads data from the given slave, always returning acknowledge bits except for the last byte.
 */
bool twi_readFromSlave(uint8_t address, uint8_t *data, size_t len);
bool twi_writeToSlaveRegister(
    uint8_t address, uint8_t reg, uint8_t *data, size_t len);
bool twi_readFromSlaveRegister(
    uint8_t address, uint8_t reg, uint8_t *data, size_t len);



#endif /* TWI_H_ */
