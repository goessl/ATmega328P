/*
 * PCA9685.c
 * 
 * Author:      Sebastian Gössl
 * Hardware:    ATmega328P, PCA9685
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



#include <util/delay.h>
#include <math.h>
#include "TWI.h"
#include "PCA9685.h"



#ifndef F_CPU
    #define F_CPU 16000000UL
    #warning "F_CPU not defined! Assuming 16MHz."
#endif


#define PCA9685_ADDRESS 0x70
#define PCA9685_FREQUENCY   25000000

#define PCA9685_SWRST_ADDRESS 0x00
#define PCA9685_SWRST_BYTE 0x06


#define PCA9685_RA_MODE1            0x00
#define PCA9685_BIT_RESTART         7
#define PCA9685_BIT_EXTCLK          6
#define PCA9685_BIT_AI              5
#define PCA9685_BIT_SLEEP           4
#define PCA9685_BIT_SUB1            3
#define PCA9685_BIT_SUB2            2
#define PCA9685_BIT_SUB3            1
#define PCA9685_BIT_ALLCALL         0

#define PCA9685_RA_MODE2            0x01
#define PCA9685_BIT_INVRT           4
#define PCA9685_BIT_OCH             3
#define PCA9685_BIT_OUTDRV          2
#define PCA9685_BIT_OUTNE_MASK      0x03

#define PCA9685_RA_SUBADR1          0x02
#define PCA9685_RA_SUBADR2          0x03
#define PCA9685_RA_SUBADR3          0x04
#define PCA9685_RA_ALLCALLADR       0x05


#define PCA9685_RA_LED0_ON_L        0x06
#define PCA9685_RA_LED0_ON_H        0x07
#define PCA9685_RA_LED0_OFF_L       0x08
#define PCA9685_RA_LED0_OFF_H       0x09

#define PCA9685_RA_LEDN_ON_L(x)     (4*x + PCA9685_RA_LED0_ON_L)
#define PCA9685_RA_LEDN_ON_H(x)     (4*x + PCA9685_RA_LED0_ON_H)
#define PCA9685_RA_LEDN_OFF_L(x)    (4*x + PCA9685_RA_LED0_OFF_L)
#define PCA9685_RA_LEDN_OFF_H(x)    (4*x + PCA9685_RA_LED0_OFF_H)


#define PCA9685_RA_ALL_LED_ON_L     0xFA
#define PCA9685_RA_ALL_LED_ON_H     0xFB
#define PCA9685_RA_ALL_LED_OFF_L    0xFC
#define PCA9685_RA_ALL_LED_OFF_H    0xFD
#define PCA9685_RA_PRE_SCALE        0xFE
#define PCA9685_RA_TEST_MODE        0xFF



static bool PCA9685_read(uint8_t address, uint8_t* data, size_t len);



bool PCA9685_init(void)
{
    (void)PCA9685_read;
    return PCA9685_reset();
}



static bool PCA9685_write(uint8_t address, uint8_t* data, size_t len)
{
    return TWI_writeToSlaveRegister(PCA9685_ADDRESS, address, data, len);
}

static bool PCA9685_read(uint8_t address, uint8_t* data, size_t len)
{
    return TWI_readFromSlaveRegister(PCA9685_ADDRESS, address, data, len);
}



bool PCA9685_reset(void)
{
    uint8_t byte = PCA9685_SWRST_BYTE;
    
    
    
    if(TWI_writeToSlave(PCA9685_SWRST_ADDRESS, &byte, 1))
        return true;
    
    byte = (1 << PCA9685_BIT_AI) | (1 << PCA9685_BIT_ALLCALL);
    if(PCA9685_write(PCA9685_RA_MODE1, &byte, 1))
        return true;
    
    return PCA9685_setFrequency(50);
}

bool PCA9685_setFrequency(double freq)
{
    uint8_t prescale, data;
    
    
    
    freq *= 0.9;  // Correct for overshoot in the frequency setting (see issue #11).
    prescale = floor(PCA9685_FREQUENCY / 4096.0 / freq) - 1;
    
    
    
    data = (1 << PCA9685_BIT_AI) | (1 << PCA9685_BIT_ALLCALL) | (1 << PCA9685_BIT_SLEEP);
    if(PCA9685_write(PCA9685_RA_MODE1, &data, 1))
        return true;
    if(PCA9685_write(PCA9685_RA_PRE_SCALE, &prescale, 1))
        return true;
    data = (1 << PCA9685_BIT_AI) | (1 << PCA9685_BIT_ALLCALL);
    if(PCA9685_write(PCA9685_RA_MODE1, &data, 1))
        return true;
    _delay_us(500);
    
    
    return false;
}


bool PCA9685_setOnOff(size_t pin, uint16_t on, uint16_t off)
{
    uint16_t temp[2] = {on, off};
    
    return PCA9685_write(PCA9685_RA_LEDN_ON_L(pin), (uint8_t*)temp, 4);
}

bool PCA9685_setPWM(size_t pin, uint16_t value, int inv)
{
    uint16_t on, off;
    
    
    
    value = fmin(value, PCA9685_MAX - 1);
    if(!inv)
    {
        if(value == 0)
            return PCA9685_setOnOff(pin, 0, PCA9685_MAX);
        else if(value == PCA9685_MAX - 1)
            return PCA9685_setOnOff(pin, PCA9685_MAX, 0);
        else
            return PCA9685_setOnOff(pin, 0, value);
    }
    else
    {
        if(value == 0)
            return PCA9685_setOnOff(pin, PCA9685_MAX, 0);
        else if(value == PCA9685_MAX - 1)
            return PCA9685_setOnOff(pin, 0, PCA9685_MAX);
        else
            return PCA9685_setOnOff(pin, 0, (PCA9685_MAX - 1) - value);
    }
    
    
    return PCA9685_setOnOff(pin, on, off);
}

bool PCA9685_setPWMScaled(size_t pin, double value, int inv)
{
    return PCA9685_setPWM(pin, PCA9685_MAX*value, inv);
}

bool PCA9685_setServo(size_t pin, double value)
{
    return PCA9685_setOnOff(pin, 0, (value*PCA9685_MAX/20.0 + PCA9685_MAX/20.0));
}
