/*
 * Servo.c
 * 
 * Author:      Sebastian Gössl
 * Hardware:    ATmega328P
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



#include <avr/io.h>
#include <avr/interrupt.h>
#include "Servo.h"



#ifndef F_CPU
    #define F_CPU 16000000UL
    #warning "F_CPU not defined! Assuming 16MHz."
#endif


#define SERVO_BASE_US (20 * 1000)
#define SERVO_MIN_US (1 * 1000)
#define SERVO_MAX_US (2 * 1000)
#define SERVO_US_PER_SECOND 1000000


#if SERVO_TIMER == 0
    #define SERVO_TIMER_TOP 0xFF
    #define SERVO_ISR_vect TIMER0_COMPA_vect
    
    #if SERVO_US_PER_SECOND / SERVO_BASE_US > F_CPU
        #error "SERVO_BASE_US to small!"
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 1 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 1
        #define CS00_VALUE 1
        #define CS01_VALUE 0
        #define CS02_VALUE 0
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 8 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 8
        #define CS00_VALUE 0
        #define CS01_VALUE 1
        #define CS02_VALUE 0
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 64 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 64
        #define CS00_VALUE 1
        #define CS01_VALUE 1
        #define CS02_VALUE 0
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 256 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 256
        #define CS00_VALUE 0
        #define CS01_VALUE 0
        #define CS02_VALUE 1
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 1024 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 1024
        #define CS00_VALUE 1
        #define CS01_VALUE 0
        #define CS02_VALUE 1
    #else
        #error "SERVO_BASE_US to long!"
    #endif
    
#elif SERVO_TIMER == 1
    #define SERVO_TIMER_TOP 0xFFFF
    #define SERVO_ISR_vect TIMER1_COMPA_vect
    
    #if SERVO_US_PER_SECOND / SERVO_BASE_US > F_CPU
        #error "SERVO_BASE_US to small!"
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 1 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 1
        #define CS10_VALUE 1
        #define CS11_VALUE 0
        #define CS12_VALUE 0
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 8 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 8
        #define CS10_VALUE 0
        #define CS11_VALUE 1
        #define CS12_VALUE 0
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 64 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 64
        #define CS10_VALUE 1
        #define CS11_VALUE 1
        #define CS12_VALUE 0
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 256 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 256
        #define CS10_VALUE 0
        #define CS11_VALUE 0
        #define CS12_VALUE 1
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 1024 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 1024
        #define CS10_VALUE 1
        #define CS11_VALUE 0
        #define CS12_VALUE 1
    #else
        #error "SERVO_BASE_US to long!"
    #endif
    
#elif SERVO_TIMER == 2
    #define SERVO_TIMER_TOP 0xFF
    #define SERVO_ISR_vect TIMER2_COMPA_vect
    
    #if SERVO_US_PER_SECOND / SERVO_BASE_US > F_CPU
        #error "SERVO_BASE_US to small!"
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 1 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 1
        #define CS20_VALUE 1
        #define CS21_VALUE 0
        #define CS22_VALUE 0
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 8 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 8
        #define CS20_VALUE 0
        #define CS21_VALUE 1
        #define CS22_VALUE 0
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 32 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 32
        #define CS20_VALUE 1
        #define CS21_VALUE 1
        #define CS22_VALUE 0
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 64 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 64
        #define CS20_VALUE 0
        #define CS21_VALUE 0
        #define CS22_VALUE 1
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 128 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 128
        #define CS20_VALUE 1
        #define CS21_VALUE 0
        #define CS22_VALUE 1
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 256 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 256
        #define CS20_VALUE 0
        #define CS21_VALUE 1
        #define CS22_VALUE 1
    #elif F_CPU * SERVO_BASE_US / SERVO_US_PER_SECOND / 1024 - 1 <= SERVO_TIMER_TOP
        #define SERVO_PRESCALER 1024
        #define CS20_VALUE 1
        #define CS21_VALUE 1
        #define CS22_VALUE 1
    #else
        #error "SERVO_BASE_US to long!"
    #endif
    
#else
    #error "No valid SERVO_TIMER selected!"
#endif



static double SERVO_percentToUs(double percent);
static void SERVO_setTimer(double us);



static volatile uint8_t** SERVO_PORTs;
static volatile uint8_t* SERVO_masks;
static volatile size_t SERVO_n;
static volatile size_t SERVO_current = 0;
static volatile double SERVO_values[SERVO_BASE_US / SERVO_MAX_US] = {0};



void SERVO_init(uint8_t** DDRs, uint8_t** PORTs, uint8_t* masks, size_t n)
{
    size_t i;
    
    
    
    SERVO_PORTs = (volatile uint8_t**)PORTs;
    SERVO_masks = (volatile uint8_t*)masks;
    SERVO_n = (volatile size_t)n;
    
    
    //Set outputs
    for(i=0; i<SERVO_n; i++)
        *DDRs[i] |= SERVO_masks[i];
    
    
    //Timer
    SERVO_setTimer(SERVO_values[0]);
    #if SERVO_TIMER == 0
        TCCR0A |= (1 << WGM01);
        TCCR0B |= (CS02_VALUE << CS02) | (CS01_VALUE << CS01) | (CS00_VALUE << CS00);
        TIMSK0 |= (1 << OCIE0A);
    #elif SERVO_TIMER == 1
        TCCR1B |= (1 << WGM12) | (CS12_VALUE << CS12) | (CS11_VALUE << CS11) | (CS10_VALUE << CS10);
        TIMSK1 |= (1 << OCIE1A);
    #elif SERVO_TIMER == 2
        TCCR2A |= (1 << WGM21);
        TCCR2B |= (CS22_VALUE << CS22) | (CS21_VALUE << CS21) | (CS20_VALUE << CS20);
        TIMSK2 |= (1 << OCIE2A);
    #endif
}


void SERVO_setServo(size_t index, double percent)
{
    if(percent > 1)
        percent = 1;
    else if(percent < 0)
        percent = 0;
    
    if(index < SERVO_n)
        SERVO_values[index] = SERVO_percentToUs(percent);
}

void SERVO_setServos(double* percent)
{
    size_t i;
    
    for(i=0; i<SERVO_n; i++)
        SERVO_setServo(i, percent[i]);
}

void SERVO_setAllServos(double percent)
{
    size_t i;
    
    for(i=0; i<SERVO_n; i++)
        SERVO_setServo(i, percent);
}


static double SERVO_percentToUs(double percent)
{
    return (SERVO_MAX_US - SERVO_MIN_US) * percent + SERVO_MIN_US;
}

static void SERVO_setTimer(double us)
{
    #if SERVO_TIMER == 0
        OCR0A = us * F_CPU / SERVO_PRESCALER / SERVO_US_PER_SECOND - 1;
    #elif SERVO_TIMER == 1
        OCR1A = us * F_CPU / SERVO_PRESCALER / SERVO_US_PER_SECOND - 1;
    #elif SERVO_TIMER == 2
        OCR2A = us * F_CPU / SERVO_PRESCALER / SERVO_US_PER_SECOND - 1;
    #endif
}



ISR(SERVO_ISR_vect)
{
    if(~*SERVO_PORTs[SERVO_current] & SERVO_masks[SERVO_current])
    {
        *SERVO_PORTs[SERVO_current] |= SERVO_masks[SERVO_current];
        SERVO_setTimer(SERVO_values[SERVO_current]);
    }
    else
    {
        *SERVO_PORTs[SERVO_current] &= ~SERVO_masks[SERVO_current];
        SERVO_setTimer(SERVO_BASE_US/SERVO_n - SERVO_values[SERVO_current]);
        
        SERVO_current++;
        if(SERVO_current >= SERVO_n)
            SERVO_current = 0;
    }
}
