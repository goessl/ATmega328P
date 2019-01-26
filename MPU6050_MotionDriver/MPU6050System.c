/*
 * MPU6050System.c
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



#ifndef F_CPU
    #define F_CPU 16000000UL
    #warning "F_CPU not defined! Assuming 16MHz."
#endif



#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "TWI.h"
#include "MPU6050System.h"



#define MPU6050SYSTEM_MS_PER_SECOND 1000

#if MPU6050SYSTEM_TIMER == 0
    #define MPU6050SYSTEM_TIMER_TOP 0xFF
    #define MPU6050SYSTEM_ISR TIMER0_COMPA_vect
    
    #if MPU6050SYSTEM_MS_PER_SECOND > F_CPU
        #error "MPU6050SYSTEM_MS_PER_SECOND to high!"
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 1 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 1
        #define CS00_VALUE 1
        #define CS01_VALUE 0
        #define CS02_VALUE 0
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 8 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 8
        #define CS00_VALUE 0
        #define CS01_VALUE 1
        #define CS02_VALUE 0
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 64 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 64
        #define CS00_VALUE 1
        #define CS01_VALUE 1
        #define CS02_VALUE 0
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 256 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 256
        #define CS00_VALUE 0
        #define CS01_VALUE 0
        #define CS02_VALUE 1
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 1024 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 1024
        #define CS00_VALUE 1
        #define CS01_VALUE 0
        #define CS02_VALUE 1
    #else
        #error "MPU6050SYSTEM_MS_PER_SECOND to small!"
    #endif
    
#elif MPU6050SYSTEM_TIMER == 1
    #define MPU6050SYSTEM_TIMER_TOP 0xFFFF
    #define MPU6050SYSTEM_ISR TIMER1_COMPA_vect
    
    #if MPU6050SYSTEM_MS_PER_SECOND > F_CPU
        #error "MPU6050SYSTEM_MS_PER_SECOND to high!"
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 1 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 1
        #define CS10_VALUE 1
        #define CS11_VALUE 0
        #define CS12_VALUE 0
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 8 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 8
        #define CS10_VALUE 0
        #define CS11_VALUE 1
        #define CS12_VALUE 0
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 64 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 64
        #define CS10_VALUE 1
        #define CS11_VALUE 1
        #define CS12_VALUE 0
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 256 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 256
        #define CS10_VALUE 0
        #define CS11_VALUE 0
        #define CS12_VALUE 1
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 1024 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 1024
        #define CS10_VALUE 1
        #define CS11_VALUE 0
        #define CS12_VALUE 1
    #else
        #error "MPU6050SYSTEM_MS_PER_SECOND to small!"
    #endif
    
#elif MPU6050SYSTEM_TIMER == 2
    #define MPU6050SYSTEM_TIMER_TOP 0xFF
    #define MPU6050SYSTEM_ISR TIMER2_COMPA_vect
    
    #if MPU6050SYSTEM_MS_PER_SECOND > F_CPU
        #error "MPU6050SYSTEM_MS_PER_SECOND to high!"
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 1 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 1
        #define CS20_VALUE 1
        #define CS21_VALUE 0
        #define CS22_VALUE 0
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 8 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 8
        #define CS20_VALUE 0
        #define CS21_VALUE 1
        #define CS22_VALUE 0
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 32 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 32
        #define CS20_VALUE 1
        #define CS21_VALUE 1
        #define CS22_VALUE 0
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 64 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 64
        #define CS20_VALUE 0
        #define CS21_VALUE 0
        #define CS22_VALUE 1
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 128 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 128
        #define CS20_VALUE 1
        #define CS21_VALUE 0
        #define CS22_VALUE 1
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 256 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 256
        #define CS20_VALUE 0
        #define CS21_VALUE 1
        #define CS22_VALUE 1
    #elif F_CPU / MPU6050SYSTEM_MS_PER_SECOND / 1024 <= MPU6050SYSTEM_TIMER_TOP
        #define MPU6050SYSTEM_PRESCALER 1024
        #define CS20_VALUE 1
        #define CS21_VALUE 1
        #define CS22_VALUE 1
    #else
        #error "MPU6050SYSTEM_MS_PER_SECOND to small!"
    #endif
    
#else
    #error "No valid MPU6050SYSTEM_TIMER selected!"
#endif



volatile unsigned long int MPU6050System_ms = 0;



void MPU6050System_init(void)
{
    #if MPU6050SYSTEM_TIMER == 0
        OCR0A = F_CPU / MPU6050SYSTEM_PRESCALER / MPU6050SYSTEM_MS_PER_SECOND;
        TCCR0A |= (1 << WGM01);
        TCCR0B |= (CS02_VALUE << CS02) | (CS01_VALUE << CS01) | (CS00_VALUE << CS00);
        TIMSK0 |= (1 << OCIE0A);
    #elif MPU6050SYSTEM_TIMER == 1
        OCR1A = F_CPU / MPU6050SYSTEM_PRESCALER / MPU6050SYSTEM_MS_PER_SECOND;
        TCCR1B |= (1 << WGM12) | (CS12_VALUE << CS12) | (CS11_VALUE << CS11) | (CS10_VALUE << CS10);
        TIMSK1 |= (1 << OCIE1A);
    #elif MPU6050SYSTEM_TIMER == 2
        OCR2A = F_CPU / MPU6050SYSTEM_PRESCALER / MPU6050SYSTEM_MS_PER_SECOND;
        TCCR2A |= (1 << WGM21);
        TCCR2B |= (CS22_VALUE << CS22) | (CS21_VALUE << CS21) | (CS20_VALUE << CS20);
        TIMSK2 |= (1 << OCIE2A);
    #endif
    
    
    TWI_init();
}


int MPU6050System_twiWrite(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const* data)
{
    return TWI_writeToSlaveRegister(slave_addr, reg_addr, (uint8_t*)data, length);
}

int MPU6050System_twiRead(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char* data)
{
    return TWI_readFromSlaveRegister(slave_addr, reg_addr, data, length);
}


int MPU6050System_delayMs(unsigned long num_ms)
{
    while(num_ms--)
        _delay_ms(1);
    
    return 0;
}

int MPU6050System_getMs(unsigned long* count)
{
    if(!count)
        return 1;
    *count = MPU6050System_ms;
    
    return 0;
}


void MPU6050System_logI(const char* fmt, ...)
{
    va_list args;
    
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void MPU6050System_logE(const char* fmt, ...)
{
    va_list args;
    
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}



ISR(MPU6050SYSTEM_ISR)
{
    MPU6050System_ms++;
}
