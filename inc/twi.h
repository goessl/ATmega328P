/*
 * twi.h
 * 
 * Minimalistic, blocking TWI driver.
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



#include <stdbool.h>    //bool type
#include <stdint.h>     //uint8_t type
#include <stddef.h>     //size_t type



//default to Fast Mode
#ifndef TWI_FREQUENCY
    #define TWI_FREQUENCY 400000
#endif



/**
 * Initializes the TWI hardware for master mode operating at TWI_FREQUENCY.
 * Configures SCL and SDA (PC5 and PC4) as outputs.
 */
void twi_init(void);



/**
 * Sends a START condition and returns 0 on success.
 * 
 * @return 0 on success, 1 otherwise
 */
bool twi_start(void);
/**
 * Sends a REPeated START condition and returns 0 on success.
 * 
 * @return 0 on success, 1 otherwise
 */
bool twi_repStart(void);
/**
 * Sends a STOP condition.
 */
void twi_stop(void);

/**
 * Sends the slave address (7 bit, SLA) with intend to write (SLA_W).
 * 
 * @param address address of the slave (7-bit)
 * @return 0 if an acknowledge bit (ACK) was returned, 1 otherwise (NACK)
 */
bool twi_addressWrite(uint8_t address);
/**
 * Sends the slave address (7 bit, SLA) with intend to read (SLA_R).
 * 
 * @param address address of the slave (7-bit)
 * @return 0 if an acknowledge bit (ACK) was returned, 1 otherwise (NACK)
 */
bool twi_addressRead(uint8_t address);

/**
 * Writes a single byte (DATA).
 * 
 * @param data byte to write
 * @return 0 if an acknowledge bit (ACK) was returned, 1 otherwise (NACK)
 */
bool twi_write(uint8_t data);
/**
 * Writes multiple bytes (DATA) until all bytes have been written
 * or no acknowledge bit (NACK) has been returned.
 * 
 * @param data location of the bytes to write
 * @param len number of bytes to write
 * @return number of bytes
 * for which acknowledge bits (ACK) have been received (len-1 on success)
 */
size_t twi_writeBurst(uint8_t *data, size_t len);
/**
 * Reads a single byte (DATA) and sends an acknowledge bit (ACK).
 * 
 * @param data location where the byte should be written to
 * @return 0 on success, 1 otherwise
 */
bool twi_readAck(uint8_t *data);
/**
 * Reads multiple data bytes (DATA) and always sends acknowledge bits (ACK)
 * until all bytes have been read or a read has failed.
 * 
 * @param data location where the bytes should be written to
 * @param len number of bytes to read
 * @return the number of bytes
 * that have been read successfully (len on success)
 */
size_t twi_readAckBurst(uint8_t *data, size_t len);
/**
 * Reads a single byte (DATA) and sends no acknowledge bit (NACK).
 * 
 * @param data location where the byte should be written to
 * @return 0 on success, 1 otherwise
 */
bool twi_readNoAck(uint8_t *data);
/**
 * Reads multiple data bytes (DATA) and never sends an acknowledge bit (NACK)
 * until all bytes have been read or a read has failed.
 * 
 * @param data location where the bytes should be written to
 * @param len number of bytes to read
 * @return the number of bytes
 * that have been read successfully (len on success)
 */
size_t twi_readNoAckBurst(uint8_t *data, size_t len);

/**
 * Writes multiple bytes to a slave (START, SLA_W, DATA, ..., DATA, STOP).
 * If sending the START condition fails, this function immediately returns 1.
 * If any failure occurs after the START condition,
 * the communication is immediately terminated with a STOP condition
 * and then this function returns 1.
 * 
 * @param address address of the slave (7-bit)
 * @param data location of the bytes to write
 * @param len number of bytes to write
 * @return 0 on success, 1 on failure
 */
bool twi_writeToSlave(uint8_t address, uint8_t *data, size_t len);
/**
 * Reads multiple bytes from a slave
 * (START, SLA_R, DATA+ACK, ..., DATA+ACK, DATA+NACK, STOP).
 * If sending the START condition fails, this function immediately returns 1.
 * If any failure occurs after the START condition,
 * the communication is immediately terminated with a STOP condition
 * and then this function returns 1.
 * 
 * @param address address of the slave (7-bit)
 * @param data location where the bytes should be written to
 * @param len number of bytes to read
 * @return 0 on success, 1 on failure
 */
bool twi_readFromSlave(uint8_t address, uint8_t *data, size_t len);
/**
 * Writes a single byte followed by multiple bytes to a slave
 * (START, SLA_W, DATA (reg), DATA, ..., DATA, STOP).
 * If sending the START condition fails, this function immediately returns 1.
 * If any failure occurs after the START condition,
 * the communication is immediately terminated with a STOP condition
 * and then this function returns 1.
 * 
 * @param address address of the slave (7-bit)
 * @param reg byte to write before the multiple bytes
 * @param data location of the bytes to write
 * @param len number of bytes to write
 * @return 0 on success, 1 on failure
 */
bool twi_writeToSlaveRegister(
    uint8_t address, uint8_t reg, uint8_t *data, size_t len);
/**
 * First writes a single byte to a slave and the reads multiple bytes from it
 * (START, SLA_W, DATA (reg),
 * START, SLA_R, DATA+ACK, ..., DATA+ACK, DATA+NACK, STOP).
 * If sending the START condition fails, this function immediately returns 1.
 * If any failure occurs after the START condition,
 * the communication is immediately terminated with a STOP condition
 * and then this function returns 1.
 * 
 * @param address address of the slave (7-bit)
 * @param reg byte to write before reading from the slave
 * @param data location where the bytes should be written to
 * @param len number of bytes to read
 * @return 0 on success, 1 on failure
 */
bool twi_readFromSlaveRegister(
    uint8_t address, uint8_t reg, uint8_t *data, size_t len);



#endif /* TWI_H_ */
