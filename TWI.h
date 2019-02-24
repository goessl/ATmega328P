/*
 * TWI.h
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



#ifndef TWI_H_
#define TWI_H_



#ifndef TWI_FREQUENCY
    #define TWI_FREQUENCY 400000
#endif



#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>



void TWI_init(void);

bool TWI_start(void);
bool TWI_repStart(void);
void TWI_stop(void);

bool TWI_addressWrite(uint8_t address);
bool TWI_addressRead(uint8_t address);

bool TWI_write(uint8_t data);
bool TWI_writeBurst(uint8_t* data, size_t len);

bool TWI_readAck(uint8_t* data);
bool TWI_readAckBurst(uint8_t* data, size_t len);
bool TWI_readNoAck(uint8_t* data);
bool TWI_readNoAckBurst(uint8_t* data, size_t len);

bool TWI_writeToSlave(uint8_t address, uint8_t* data, size_t len);
bool TWI_readFromSlave(uint8_t address, uint8_t* data, size_t len);
bool TWI_writeToSlaveRegister(
    uint8_t address, uint8_t reg, uint8_t* data, size_t len);
bool TWI_readFromSlaveRegister(
    uint8_t address, uint8_t reg, uint8_t* data, size_t len);



#endif /* TWI_H_ */
