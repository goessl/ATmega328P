/*
 * pid.c
 * 
 * PID controller module.
 * 
 * Author:      Sebastian Goessl
 * Hardware:    ATmega328P
 * 
 * LICENSE:
 * MIT License
 * 
 * Copyright (c) 2019 Sebastian Goessl
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



#include <avr/io.h>         //hardware registers
#include <avr/interrupt.h>  //interrupt vectors
#include <util/atomic.h>    //atomic blocks
#include "pid.h"



//default to Arduino oscillator
#ifndef F_CPU
    #define F_CPU 16000000UL
    #warning "F_CPU not defined! Assuming 16MHz."
#endif



//returns the value v clamped between min and max
#define PID_CLAMP(v, min, max) \
    (((v)<(min)) ? (min) : (((v)>(max)) ? (max) : (v)))



/** Timer overflows since the last iteration. */
static volatile uint16_t pid_overflows = 0;
/** Registered PID controllers. */
static volatile Pid_t *pid_controllers;
/** Number of registered PID controllers. */
static volatile size_t pid_n;



void pid_init(Pid_t *controllers, size_t n)
{
    pid_controllers = controllers;
    pid_n = n;
    
    
    
    //start selected timer prescaled to 1 and with overflow interrupt
    #if PID_TIMER == 0
        
        TCCR0B |= (1 << CS00);
        TIMSK0 |= (1 << TOIE0);
        #define PID_TIMER_TCNT TCNT0
        #define PID_TIMER_TOP 0xFF
        #define PID_vect TIMER0_OVF_vect
        
    #elif PID_TIMER == 1
        
        TCCR1B |= (1 << CS10);
        TIMSK1 |= (1 << TOIE1);
        #define PID_TIMER_TCNT TCNT1
        #define PID_TIMER_TOP 0xFFFF
        #define PID_vect TIMER1_OVF_vect
        
    #elif PID_TIMER == 2
        
        TCCR2B |= (1 << CS20);
        TIMSK2 |= (1 << TOIE2);
        #define PID_TIMER_TCNT TCNT2
        #define PID_TIMER_TOP 0xFF
        #define PID_vect TIMER2_OVF_vect
    #else
        #error "No valid PID_TIMER selected!"
    #endif
}

Pid_t pid_initController(double *w, double *r, double *u,
    double kp, double ki, double kd,
    double iMax, double dMax, double outMax)
{
    return PID_INIT_CONTROLLER(w, r, u, kp, ki, kd, iMax, dMax, outMax);
}



/**
 * Update a singe pid controller.
 * 
 * @param controller pointer to the PID controler to update
 * @param dt elapsed time in seconds since the last iteration
 */
static void pid_iterateSingle(Pid_t *controller, double dt)
{
    double derivative, u;
    double e = *controller->w - *controller->r;
    
    
    
    controller->sum += e * dt;
    controller->sum = PID_CLAMP(controller->sum, -controller->iMax, controller->iMax);
    
    derivative = (e - controller->last) / dt;
    derivative = PID_CLAMP(derivative, -controller->dMax, controller->dMax);
    controller->last = e;
    
    
    u = controller->kp * e
        + controller->ki * controller->sum
        + controller->kd * derivative;
    *controller->u = PID_CLAMP(u, -controller->outMax, controller->outMax);
}



uint32_t pid_iterate(void)
{
    size_t i;
    uint32_t ticks;
    double dt;
    
    
    
    //read out timer and reset counter
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ticks = (uint32_t)PID_TIMER_TOP * pid_overflows + PID_TIMER_TCNT;
        PID_TIMER_TCNT = 0;
        pid_overflows = 0;
    }
    
    dt = (double)ticks / F_CPU;
    
    
    //update
    for(i=0; i<pid_n; i++)
        pid_iterateSingle((Pid_t*)&pid_controllers[i], dt);
    
    
    return ticks;
}



//overflow interrupt
//16MHz / 256 = 62.5kHz overflows
//62.5kHz / 65536 = overflow counter limit reached at approx 0.95Hz
ISR(PID_vect)
{
    if(pid_overflows < UINT16_MAX)
        pid_overflows++;
}
