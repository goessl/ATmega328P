/*
 * PID.c
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
#include "PID.h"



#ifndef F_CPU
    #define F_CPU 16000000UL
    #warning "F_CPU not defined! Assuming 16MHz."
#endif

#define PID_CLAMP(v, min, max) ((v<min)?min:((v>max)?max:v))



static volatile double PID_dt;
static volatile PID_t* PID_controllers;
static volatile size_t PID_n;
static void (*PID_cb)(void);



void PID_init(uint32_t frequency, PID_t* controllers, size_t n,
    void (*cb)(void))
{
    uint8_t csValue = 0;
    uint16_t prescaler = 0;
    
    
    
    PID_controllers = controllers;
    PID_n = n;
    PID_dt = 1.0 / frequency;
    PID_cb = cb;
    
    
    
    #if PID_TIMER == 0
    
        if(F_CPU / frequency / 1 - 1 <= 0xFF) {
            prescaler = 1;
            csValue = (1 << CS00);
        } else if(F_CPU / frequency / 8 - 1 <= 0xFF) {
            prescaler = 8;
            csValue = (1 << CS01);
        } else if(F_CPU / frequency / 64 - 1 <= 0xFF) {
            prescaler =  64;
            csValue = (1 << CS01) | (1 << CS00);
        } else if(F_CPU / frequency / 256 - 1 <= 0xFF) {
            prescaler = 256;
            csValue = (1 << CS02);
        } else if(F_CPU / frequency / 1024 - 1 <= 0xFF) {
            prescaler = 1024;
            csValue = (1 << CS02) | (1 << CS00);
        }
        
        OCR0A = F_CPU / frequency / prescaler - 1;
        TCCR0A |= (1 << WGM01);
        TCCR0B |= csValue;
        TIMSK0 |= (1 << OCIE0A);
        #define PID_vect TIMER0_COMPA_vect
        
        
    #elif PID_TIMER == 1
        
        if(F_CPU / frequency / 1 - 1 <= 0xFFFF) {
            prescaler = 1;
            csValue = (1 << CS10);
        } else if(F_CPU / frequency / 8 - 1 <= 0xFFFF) {
            prescaler = 8;
            csValue = (1 << CS11);
        } else if(F_CPU / frequency / 64 - 1 <= 0xFFFF) {
            prescaler = 64;
            csValue = (1 << CS11) | (1 << CS10);
        } else if(F_CPU / frequency / 256 - 1 <= 0xFFFF) {
            prescaler = 256;
            csValue = (1 << CS12);
        } else if(F_CPU / frequency / 1024 - 1 <= 0xFFFF) {
            prescaler = 1024;
            csValue = (1 << CS12) | (1 << CS10);
        }
        
        OCR1A = F_CPU / frequency / prescaler - 1;
        TCCR1B |= (1 << WGM12) | csValue;
        TIMSK1 |= (1 << OCIE1A);
        #define PID_vect TIMER1_COMPA_vect
        
        
    #elif PID_TIMER == 2
        
        if(F_CPU / frequency / 1 - 1 <= 0xFF) {
            prescaler = 1;
            csValue = (1 << CS20);
        } else if(F_CPU / frequency / 8 - 1 <= 0xFF) {
            prescaler = 8;
            csValue = (1 << CS21);
        } else if(F_CPU / frequency / 32 - 1 <= 0xFF) {
            prescaler = 32;
            csValue = (1 << CS21) | (1 << CS20);
        } else if(F_CPU / frequency / 64 - 1 <= 0xFF) {
            prescaler = 64;
            csValue = (1 << CS22);
        } else if(F_CPU / frequency / 128 - 1 <= 0xFF) {
            prescaler = 128;
            csValue = (1 << CS22) | (1 << CS20);
        } else if(F_CPU / frequency / 256 - 1 <= 0xFF) {
            prescaler = 256;
            csValue = (1 << CS22) | (1 << CS21);
        } else if(F_CPU / frequency / 1024 - 1 <= 0xFF) {
            prescaler = 1024;
            csValue = (1 << CS22) | (1 << CS21) | (1 << CS20);
        }
        
        OCR2A = F_CPU / frequency / prescaler - 1;
        TCCR2A |= (1 << WGM21);
        TCCR2B |= csValue;
        TIMSK2 |= (1 << OCIE2A);
        #define PID_vect TIMER2_COMPA_vect
    #endif
}

PID_t PID_initController(double* w, double* y, double* x,
    double kp, double ki, double kd,
    double iMax, double dMax, double outMax)
{
    return (PID_t){.w = w, .y = y, .x = x,
        .kp = kp, .ki = ki, .kd = kd,
        .sum = 0, .last = 0,
        .iMax = iMax, .dMax = dMax, .outMax = outMax};
}



static void PID_iterate(PID_t* controller)
{
    double derivative, y;
    double e = *controller->w - *controller->x;
    
    
    
    controller->sum += e * PID_dt;
    controller->sum =
        PID_CLAMP(controller->sum, -controller->iMax, controller->iMax);
    
    derivative = (e - controller->last) / PID_dt;
    derivative = PID_CLAMP(derivative, -controller->dMax, controller->dMax);
    controller->last = e;
    
    
    y = controller->kp * e
        + controller->ki * controller->sum
        + controller->kd * derivative;
    *controller->y = PID_CLAMP(y, -controller->outMax, controller->outMax);
}



ISR(PID_vect)
{
    size_t i;
    
    for(i=0; i<PID_n; i++)
        PID_iterate((PID_t*)&PID_controllers[i]);
    
    if(PID_cb)
        PID_cb();
}
