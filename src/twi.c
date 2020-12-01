/*
 * twi.c
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



#include <avr/io.h>     //hardware registers
#include <util/twi.h>   //TWI status masks
#include "twi.h"



//default to Arduino oscillator
#ifndef F_CPU
    #define F_CPU 16000000UL
    #warning "F_CPU not defined! Assuming 16MHz."
#endif


//set prescaler so that the TWBR value is as large as possible
//and at least 10 (frequency error below 5%)
//https://www.nongnu.org/avr-libc/user-manual/group__twi__demo.html Note[5]
#if (F_CPU/TWI_FREQUENCY - 16) / (2 * 1) >= 10 \
        && (F_CPU/TWI_FREQUENCY - 16) / (2 * 1) <= 0xFF
    #define TWI_PRESCALER 1
    #define TWPS0_VALUE 0
    #define TWPS1_VALUE 0
#elif (F_CPU/TWI_FREQUENCY - 16) / (2 * 4) >= 10 \
        && (F_CPU/TWI_FREQUENCY - 16) / (2 * 4) <= 0xFF
    #define TWI_PRESCALER 4
    #define TWPS0_VALUE 1
    #define TWPS1_VALUE 0
#elif (F_CPU/TWI_FREQUENCY - 16) / (2 * 16) >= 10 \
        && (F_CPU/TWI_FREQUENCY - 16) / (2 * 16) <= 0xFF
    #define TWI_PRESCALER 16
    #define TWPS0_VALUE 0
    #define TWPS1_VALUE 1
#elif (F_CPU/TWI_FREQUENCY - 16) / (2 * 64) >= 10 \
        && (F_CPU/TWI_FREQUENCY - 16) / (2 * 64) <= 0xFF
    #define TWI_PRESCALER 64
    #define TWPS0_VALUE 1
    #define TWPS1_VALUE 1
#else
    #error "TWI_FREQUENCY too low!"
#endif

#define TWBR_VALUE ((F_CPU/TWI_FREQUENCY - 16) / (2 * TWI_PRESCALER))


/** Mask TWI slave addressing byte with given id and write intend. */
#define TWI_ADDRESS_W(id)    (((id) << 1) & ~0x01)
/** Mask TWI slave addressing byte  with given id and read intend. */
#define TWI_ADDRESS_R(id)    (((id) << 1) | 0x01)



void twi_init(void)
{
    TWBR = TWBR_VALUE;
    TWSR = (TWPS1_VALUE << TWPS1) | (TWPS0_VALUE << TWPS0);
    
    TWCR = (1 << TWEN);
}



/**
 * Blocks until the current condition is completed.
 */
static void twi_waitForComplete(void)
{
    while(~TWCR & (1 << TWINT))
        ;
}



bool twi_start(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    twi_waitForComplete();
    
    return TW_STATUS != TW_START;
}

bool twi_repStart(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    twi_waitForComplete();
    
    return TW_STATUS != TW_REP_START;
}

void twi_stop(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}


bool twi_addressWrite(uint8_t address)
{
    TWDR = TWI_ADDRESS_W(address);
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    twi_waitForComplete();
    
    return TW_STATUS != TW_MT_SLA_ACK;
}

bool twi_addressRead(uint8_t address)
{
    TWDR = TWI_ADDRESS_R(address);
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    twi_waitForComplete();
    
    return TW_STATUS != TW_MR_SLA_ACK;
}


bool twi_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    twi_waitForComplete();
    
    return TW_STATUS != TW_MT_DATA_ACK;
}

size_t twi_writeBurst(uint8_t *data, size_t len)
{
    size_t i = len;
    
    while(i)
    {
        if(twi_write(*data++))
            break;
        i--;
    }
    
    return len - i;
}


bool twi_readAck(uint8_t *data)
{
    TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
    twi_waitForComplete();
    
    *data = TWDR;
    
    return TW_STATUS != TW_MR_DATA_ACK;
}

size_t twi_readAckBurst(uint8_t *data, size_t len)
{
    size_t i = len;
    
    while(i)
    {
        if(twi_readAck(data++))
            break;
        i--;
    }
    
    return len - i;
}

bool twi_readNoAck(uint8_t *data)
{
    TWCR = (1 << TWINT) | (1 << TWEN);
    twi_waitForComplete();
    
    *data = TWDR;
    
    return TW_STATUS != TW_MR_DATA_NACK;
}

size_t twi_readNoAckBurst(uint8_t *data, size_t len)
{
    size_t i = len;
    
    while(i)
    {
        if(twi_readNoAck(data++))
            break;
        i--;
    }
    
    return len - i;
}


bool twi_writeToSlave(uint8_t address, uint8_t *data, size_t len)
{
    if(twi_start())
        return 1;
    if(twi_addressWrite(address))
    {
        twi_stop();
        return 1;
    }
    
    if(twi_writeBurst(data, len) != len)
    {
        twi_stop();
        return 1;
    }
    
    twi_stop();
    
    return 0;
}

bool twi_readFromSlave(uint8_t address, uint8_t *data, size_t len)
{
    if(twi_start())
        return 1;
    if(twi_addressRead(address))
    {
        twi_stop();
        return 1;
    }
    
    if(len>1 && twi_readAckBurst(data, len-1)!=len-1)
    {
        twi_stop();
        return 1;
    }
    if(len>0 && twi_readNoAck(&data[len-1]))
    {
        twi_stop();
        return 1;
    }
    
    twi_stop();
    
    return 0;
}

bool twi_writeToSlaveRegister(uint8_t address, uint8_t reg,
    uint8_t *data, size_t len)
{
    if(twi_start())
        return 1;
    if(twi_addressWrite(address))
    {
        twi_stop();
        return 1;
    }
    
    if(twi_write(reg))
    {
        twi_stop();
        return 1;
    }
    if(twi_writeBurst(data, len) != len)
    {
        twi_stop();
        return 1;
    }
    
    twi_stop();
    
    return 0;
}

bool twi_readFromSlaveRegister(uint8_t address, uint8_t reg,
    uint8_t *data, size_t len)
{
    if(twi_start())
        return 1;
    if(twi_addressWrite(address))
    {
        twi_stop();
        return 1;
    }
    if(twi_write(reg))
    {
        twi_stop();
        return 1;
    }
    
    
    if(twi_repStart())
        return 1;
    if(twi_addressRead(address))
    {
        twi_stop();
        return 1;
    }
    
    if(len>1 && twi_readAckBurst(data, len-1)!=len-1)
    {
        twi_stop();
        return 1;
    }
    if(len>0 && twi_readNoAck(&data[len-1]))
    {
        twi_stop();
        return 1;
    }
    
    twi_stop();
    
    return 0;
}
