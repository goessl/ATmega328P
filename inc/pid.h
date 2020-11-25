/*
 * pid.h
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



#ifndef PID_H_
#define PID_H_



#include <stddef.h>
#include <stdint.h>



#ifndef PID_TIMER
    #define PID_TIMER 2
#endif



typedef struct
{
    double *w;
    double *r;
    double *u;
    
    double kp, ki, kd;
    
    double sum, last;
    double iMax, dMax, outMax;
} Pid_t;



#define PID_INIT_CONTROLLER(_w, _r, _u, _kp, _ki, _kd, _iMax, _dMax, _outMax) \
    ((Pid_t){.w = (_w), .r = (_r), .u = (_u), \
        .kp = (_kp), .ki = (_ki), .kd = (_kd), \
        .sum = 0, .last = 0, \
        .iMax = (_iMax), .dMax = (_dMax), .outMax = (_outMax)})



void pid_init(Pid_t *controllers, size_t n);

Pid_t pid_initController(double *w, double *r, double *u,
    double kp, double ki, double kd,
    double iMax, double dMax, double outMax);

uint32_t pid_iterate(void);



#endif /* PID_H_ */
