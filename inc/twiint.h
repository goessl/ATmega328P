/*
 * twiint.h
 * 
 * Buffered, interrupt based TWI driver.
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



#include <stdbool.h>    //bool type
#include <stddef.h>     //size_t type
#include <stdint.h>     //uint8_t type



//default to Fast Mode
#ifndef TWI_FREQUENCY
    #define TWI_FREQUENCY 400000
#endif

/** Slave address with write intend. */
#define TWI_ADDRESS_W(x)    (((x) << 1) & ~0x01)
/** Slave address with read intend. */
#define TWI_ADDRESS_R(x)    (((x) << 1) | 0x01)



/**
 * Initializes the TWI hardware for master mode operating at TWI_FREQUENCY.
 * Configures SCL and SDA (PC5 and PC4) as outputs.
 * Interrupts have to be enabled.
 */
void twiint_init(void);

/**
 * Returns true if currently a transmission is ongoing.
 * 
 * @return if currently a transmission is ongoing
 */
bool twiint_busy(void);
/**
 * Blocks until the transmission is completed.
 */
void twiint_flush(void);

/**
 * Starts a TWI transmission writing or reading multiple bytes.
 * The address byte should be provided already manipulated by
 * TWI_ADDRESS_W if data should be written
 * or TWI_ADDRESS_R if data should be read,
 * both slave address in 7-bit format.
 * This function then returns while the transmission continues,
 * getting handled by interrupts routines.
 * The data location must not be changed while the transmission is ongoing
 * as the bytes are read/written as they are needed.
 * When an transmission is still ongoing, this function blocks until the
 * transmission is completed before starting a new one.
 * 
 * @param address slave address byte, use the result of TWI_ADDRESS_W for write
 * intend or TWI_ADDRESS_R for read intend applied on the 7-bit address
 * @param data location of the bytes to transmit
 * or location for the read in bytes to be written to
 * @param len number of bytes to write/read
 */
void twiint_start(uint8_t address, uint8_t *data, size_t len);



#endif /* TWIINT_H_ */
