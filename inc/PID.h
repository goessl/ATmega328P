/*
 * PID.h
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



#ifndef PID_H_
#define PID_H_



#include <stddef.h>
#include <stdint.h>



#ifndef PID_TIMER
    #define PID_TIMER 2
#endif



typedef struct
{
    double* w;
    double* y;
    double* x;
    
    double kp, ki, kd;
    
    double sum, last;
    double iMax, dMax, outMax;
} PID_t;



#define PID_INIT_CONTROLLER(w_, y_, x_, kp_, ki_, kd_, iMax_, dMax_, outMax_) \
    ((PID_t){.w = (w_), .y = (y_), .x = (x_), \
        .kp = (kp_), .ki = (ki_), .kd = (kd_), \
        .sum = 0, .last = 0, \
        .iMax = (iMax_), .dMax = (dMax_), .outMax = (outMax_)})



void PID_init(uint32_t frequency, PID_t* controllers, size_t n,
    void (*cb)(void));

PID_t PID_initController(double* w, double* y, double* x,
    double kp, double ki, double kd,
    double iMax, double dMax, double outMax);



#endif /* PID_H_ */
