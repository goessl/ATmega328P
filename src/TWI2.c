/*
 * TWI2.c
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



#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include "TWI2.h"



#ifndef F_CPU
    #define F_CPU 16000000UL
    #warning "F_CPU not defined! Assuming 16MHz."
#endif


#if (F_CPU/TWI_FREQUENCY - 16) / (2 * 1) <= 0xFF
    #define TWI_PRESCALER 1
    #define TWPS0_VALUE 0
    #define TWPS1_VALUE 0
#elif (F_CPU/TWI_FREQUENCY - 16) / (2 * 4) <= 0xFF
    #define TWI_PRESCALER 4
    #define TWPS0_VALUE 1
    #define TWPS1_VALUE 0
#elif (F_CPU/TWI_FREQUENCY - 16) / (2 * 16) <= 0xFF
    #define TWI_PRESCALER 16
    #define TWPS0_VALUE 0
    #define TWPS1_VALUE 1
#elif (F_CPU/TWI_FREQUENCY - 16) / (2 * 64) <= 0xFF
    #define TWI_PRESCALER 64
    #define TWPS0_VALUE 1
    #define TWPS1_VALUE 1
#else
    #error "TWI_FREQUENCY too low!"
#endif

#define TWBR_VALUE ((F_CPU/TWI_FREQUENCY - 16) / (2 * TWI_PRESCALER))



static uint8_t TWI2_address;
static uint8_t* TWI2_data;
static size_t TWI2_index;
static size_t TWI2_len;



void TWI2_init(void)
{
    TWBR = TWBR_VALUE;
    TWSR = (TWPS1_VALUE << TWPS1) | (TWPS0_VALUE << TWPS0);
    
    TWCR = (1 << TWINT) | (1 << TWEN);
}



bool TWI2_busy(void)
{
    return TWCR & (1<<TWIE);
}

void TWI2_flush(void)
{
    while(TWCR & (1<<TWIE))
        ;
}



void TWI2_start(uint8_t address, uint8_t* data, size_t len)
{
    TWI2_flush();
    
    TWI2_address = address;
    TWI2_data = data;
    TWI2_len = len;
    
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
}



ISR(TWI_vect)
{
    switch(TW_STATUS)
    {
        case TW_START:
        case TW_REP_START:
            TWI2_index = 0;
            TWDR = TWI2_address;
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
            break;
        
        
        case TW_MT_SLA_ACK:
        case TW_MT_DATA_ACK:
            if(TWI2_index < TWI2_len)
            {
                TWDR = TWI2_data[TWI2_index++];
                TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
            }
            else
            {
                TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
            }
            break;
        
        
        case TW_MR_DATA_ACK:
            TWI2_data[TWI2_index++] = TWDR;
        case TW_MR_SLA_ACK:
            if(TWI2_index < TWI2_len-1)
            {
                TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
            }
            else
            {
                TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
            }
            break;
        
        case TW_MR_DATA_NACK:
            TWI2_data[TWI2_index++] = TWDR;
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
            break;
        
        
        
        case TW_MT_ARB_LOST:
        //case TW_MR_ARB_LOST:
            TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
            break;
        
        
        
        case TW_MT_SLA_NACK:
        case TW_MT_DATA_NACK:
        case TW_MR_SLA_NACK:
        default:
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
    }
}
