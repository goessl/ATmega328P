/*
 * pid.h
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



#ifndef PID_H_
#define PID_H_



#include <stddef.h> //size_t type
#include <stdint.h> //uint32_t type



//default to timer 2
//needed for time measurement
#ifndef PID_TIMER
    #define PID_TIMER 2
#endif



/**
 * PID controller handler.
 */
typedef struct
{
    /** Set point. */
    double *w;
    /** Process variable. */
    double *r;
    /** Control variable. */
    double *u;
    
    /** Direct proportional, integral and derivative factor. */
    double kp, ki, kd;
    
    /** Integral and last error value. */
    double sum, last;
    /** Integral, derivative (both before ki and kd) and output clamp. */
    double iMax, dMax, outMax;
} Pid_t;



/**
 * Initializer function macro version.
 * Use only if really needed.
 */
#define PID_INIT_CONTROLLER(_w, _r, _u, _kp, _ki, _kd, _iMax, _dMax, _outMax) \
    ((Pid_t){.w = (_w), .r = (_r), .u = (_u), \
        .kp = (_kp), .ki = (_ki), .kd = (_kd), \
        .sum = 0, .last = 0, \
        .iMax = (_iMax), .dMax = (_dMax), .outMax = (_outMax)})



/**
 * Starts the PID_TIMER and internally saves a pointer to the controllers
 * that will be updated each iteration.
 * 
 * @param controllers array of controllers that will be updated each iteration
 * @param n number of controllers
 */
void pid_init(Pid_t *controllers, size_t n);

/**
 * Initializes a new pid controller handler
 * with the given pointers and parameters.
 * 
 * @param w pointer to the set point variable (input)
 * @param r pointer to the process variable (input)
 * @param u pointer to the control variable (output)
 * @param kp proportional factor
 * @param ki integral factor
 * @param kd derivative factor
 * @param iMax absolute integral clamp (before ki)
 * @param dMax absolute derivative clamp (before kd)
 * @param outMax absolute output clamp
 * @return a new PID controller handler
 */
Pid_t pid_initController(double *w, double *r, double *u,
    double kp, double ki, double kd,
    double iMax, double dMax, double outMax);

/**
 * Each PID controller reads its set point and process variable
 * and updates its control variable.
 * The elapsed ticks (at F_CPU frequency)
 * since the last iteration are returned.
 * 
 * @return the elapsed ticks (at F_CPU frequency) since the last iteration
 */
uint32_t pid_iterate(void);



#endif /* PID_H_ */
