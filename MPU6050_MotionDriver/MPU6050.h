/*
 * MPU6050.h
 * 
 * Author:      Sebastian Gössl
 * Hardware:    ATmega328P, MPU6050
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



#ifndef MPU6050_H_
#define MPU6050_H_



#define MPU6050_INTERRUPT 0


#define MPU6050_GYRO_FSR 2000
#define MPU6050_ACCEL_FSR 2
#define MPU6050_SAMPLE_RATE 200
#define MPU6050_LPF 42



#include <stdint.h>



int MPU6050_init(void);

int MPU6050_dataAvailable(void);

void MPU6050_readAllRaw(int16_t* g, int16_t* a, int32_t* q, unsigned long* timestamp, uint8_t* more);
void MPU6050_readAllScaled(double* g, double* a, double* q, unsigned long* timestamp, uint8_t* more);

void MPU6050_quaternionToGravity(double* q, double* g);
void MPU6050_quaternionToEuler(double* q, double* e);

void MPU6050_readEuler(double* e, unsigned long* timestamp, uint8_t* more);



#endif /* MPU6050_H_ */
