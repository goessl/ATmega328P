/*
 * PCA9685.h
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



#ifndef PCA9685_H_
#define PCA9685_H_



#define PCA9685_MAX (1 << 12)



#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>



bool PCA9685_init(void);

bool PCA9685_reset(void);
bool PCA9685_setFrequency(double freq);

bool PCA9685_setOnOff(size_t pin, uint16_t on, uint16_t off);
bool PCA9685_setPWM(size_t pin, uint16_t value, int inv);
bool PCA9685_setPWMScaled(size_t pin, double value, int inv);
bool PCA9685_setServo(size_t pin, double value);



#endif /* PCA9685_H_ */
