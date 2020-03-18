/*
 * Servo_main.c
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
#include <util/delay.h>
#include "Servo.h"



void init(void);



#define SERVO_N 4

uint8_t* DDRs[SERVO_N] = {
    (uint8_t*)&DDRB,
    (uint8_t*)&DDRB,
    (uint8_t*)&DDRB,
    (uint8_t*)&DDRB};
uint8_t* PORTS[SERVO_N] = {
    (uint8_t*)&PORTB,
    (uint8_t*)&PORTB,
    (uint8_t*)&PORTB,
    (uint8_t*)&PORTB};
uint8_t masks[SERVO_N] = {
    (uint8_t)(1 << PORTB5),
    (uint8_t)(1 << PORTB4),
    (uint8_t)(1 << PORTB3),
    (uint8_t)(1 << PORTB2)};



int main(void)
{
    size_t i;
    uint8_t servos[SERVO_N] = {0x00, 0xFF/4, 0xFF/2, 0xFF/4*3};
    
    
    
    init();
    
    while(1)
    {
        SERVO_setServos(servos);
        for(i=0; i<SERVO_N; i++)
        {
            servos[i]++;
        }
        
        _delay_ms(50);
    }
}

void init(void)
{
    SERVO_init(DDRs, PORTS, masks, SERVO_N);
    sei();
}
