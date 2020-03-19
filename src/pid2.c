/*
 * pid2.c
 * 
 * Author:      Sebastian Gössl
 * Hardware:    ATmega328P
 * 
 * LICENSE:
 * MIT License
 * 
 * Copyright (c) 2019 Sebastian Gössl
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
#include <util/atomic.h>
#include "pid2.h"



#ifndef F_CPU
    #define F_CPU 16000000UL
    #warning "F_CPU not defined! Assuming 16MHz."
#endif

#define PID2_CLAMP(v, min, max) \
    (((v)<(min)) ? (min) : (((v)>(max)) ? (max) : (v)))



static volatile uint16_t pid2_overflows = 0;
static volatile pid_t* pid2_controllers;
static volatile size_t pid2_n;



void pid2_init(pid_t* controllers, size_t n)
{
    pid2_controllers = controllers;
    pid2_n = n;
    
    
    
    #if PID2_TIMER == 0
        
        TCCR0B |= (1 << CS00);
        TIMSK0 |= (1 << TOIE0);
        #define PID2_TIMER_TCNT TCNT0
        #define PID2_TIMER_TOP 0xFF
        #define PID2_vect TIMER0_OVF_vect
        
    #elif PID2_TIMER == 1
        
        TCCR1B |= (1 << CS10);
        TIMSK1 |= (1 << TOIE1);
        #define PID2_TIMER_TCNT TCNT1
        #define PID2_TIMER_TOP 0xFFFF
        #define PID2_vect TIMER1_OVF_vect
        
    #elif PID2_TIMER == 2
        
        TCCR2B |= (1 << CS20);
        TIMSK2 |= (1 << TOIE2);
        #define PID2_TIMER_TCNT TCNT2
        #define PID2_TIMER_TOP 0xFF
        #define PID2_vect TIMER2_OVF_vect
    #endif
}

pid_t pid2_initController(double* w, double* y, double* x,
    double kp, double ki, double kd,
    double iMax, double dMax, double outMax)
{
    return PID2_INIT_CONTROLLER(w, y, x, kp, ki, kd, iMax, dMax, outMax);
}



static void pid2_iterateSingle(pid_t* controller, double dt)
{
    double derivative, y;
    double e = *controller->w - *controller->x;
    
    
    
    controller->sum += e * dt;
    controller->sum = PID2_CLAMP(controller->sum, -controller->iMax, controller->iMax);
    
    derivative = (e - controller->last) / dt;
    derivative = PID2_CLAMP(derivative, -controller->dMax, controller->dMax);
    controller->last = e;
    
    
    y = controller->kp * e
        + controller->ki * controller->sum
        + controller->kd * derivative;
    *controller->y = PID2_CLAMP(y, -controller->outMax, controller->outMax);
}



uint32_t pid2_iterate(void)
{
    size_t i;
    uint32_t ticks;
    double dt;
    
    
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ticks = (uint32_t)PID2_TIMER_TOP * pid2_overflows + PID2_TIMER_TCNT;
        PID2_TIMER_TCNT = 0;
        pid2_overflows = 0;
    }
    
    dt = (double)ticks / F_CPU;
    
    for(i=0; i<pid2_n; i++)
        pid2_iterateSingle((pid_t*)&pid2_controllers[i], dt);
    
    
    return ticks;
}



ISR(PID2_vect)
{
    if(pid2_overflows < UINT16_MAX)
        pid2_overflows++;
}
