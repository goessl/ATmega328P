/*
 * MPU6050System.h
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



#ifndef MPU6050SYSTEM_H_
#define MPU6050SYSTEM_H_



#define MPU6050SYSTEM_TIMER 0



void MPU6050System_init(void);

int MPU6050System_twiWrite(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const* data);
int MPU6050System_twiRead(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char* data);

int MPU6050System_delayMs(unsigned long num_ms);
int MPU6050System_getMs(unsigned long* count);

void MPU6050System_logI(const char* fmt, ...);
void MPU6050System_logE(const char* fmt, ...);



#endif /* MPU6050SYSTEM_H_ */
