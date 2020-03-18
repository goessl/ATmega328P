/*
 * Servo.h
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



#ifndef SERVO_H_
#define SERVO_H_



#include <stddef.h>
#include <stdint.h>



#ifndef SERVO_TIMER
    #define SERVO_TIMER 1
#endif



void SERVO_init(uint8_t** DDRs, uint8_t** PORTs, uint8_t* MASKS, size_t n);

void SERVO_setServo(size_t index, uint8_t value);
void SERVO_setServoScaled(size_t index, double percent);
void SERVO_setServos(uint8_t* values);
void SERVO_setServosScaled(double* percents);
void SERVO_setAllServos(uint8_t value);
void SERVO_setAllServosScaled(double percent);



#endif /* SERVO_H_ */
