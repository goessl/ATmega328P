/*
 * esc.c
 * 
 * Generic ESC driver.
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



#include <util/delay.h> //delay function for initialization
#include "servo.h"
#include "esc.h"



void esc_init(uint8_t **DDRs, uint8_t **PORTs, uint8_t *masks, size_t n)
{
    servo_init(DDRs, PORTs, masks, n);
    
    servo_setAllServos(0x00);
    _delay_ms(4000);
}

void esc_initThrottle(uint8_t **DDRs, uint8_t **PORTs, uint8_t *masks, size_t n)
{
    servo_init(DDRs, PORTs, masks, n);
    
    servo_setAllServos(0xFF);
    _delay_ms(4000);
    
    servo_setAllServos(0x00);
    _delay_ms(4000);
}



void esc_setMotor(size_t index, uint8_t value)
{
    servo_setServo(index, value);
}

void esc_setMotorScaled(size_t index, double percent)
{
    servo_setServoScaled(index, percent);
}

void esc_setMotors(uint8_t *values)
{
    servo_setServos(values);
}

void esc_setMotorsScaled(double *percents)
{
    servo_setServosScaled(percents);
}

void esc_setAllMotors(uint8_t value)
{
    servo_setAllServos(value);
}

void esc_setAllMotorsScaled(double percent)
{
    servo_setAllServosScaled(percent);
}
