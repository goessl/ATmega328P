/*
 * adc.h
 * 
 * Continuously sampling ADC driver.
 * 
 * Author:      Sebastian Goessl
 * Hardware:    ATmega328P
 * 
 * LICENSE:
 * MIT License
 * 
 * Copyright (c) 2018 Sebastian Goessl
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



#ifndef ADC_H_
#define ADC_H_



#include <stddef.h> //size_t type
#include <stdint.h> //uint16_t type



/** Number of channels */
#define ADC_N 8
/** Maximum ADC frequency. */
#define ADC_FREQUENCY_MAX 200000
/** Minimum ADC frequency. */
#define ADC_FREQUENCY_MIN 50000
/** Maximum ADC value (10bit). */
#define ADC_TOP 0x3FF



/**
 * Initializes the ADC hardware to continuously sample all channels
 * (ADC_N many) in ascending order.
 * Interrupts have to be enabled (by calling avr/interrupt.h's sei).
 */
void adc_init(void);

/**
 * Returns the last sampled value of the channel as raw unsigned integer.
 * 0 represents GND, ADC_TOP represents AREF.
 * 
 * @param channel channel index
 * @return last sampled value
 */
uint16_t adc_get(size_t channel);
/**
 * Returns the last sampled value of the channel as floating point value.
 * 0 represents GND, 1 (included) represents AREF.
 * 
 * @param channel channel index
 * @return last sampled value
 */
double adc_getScaled(size_t channel);
/**
 * Writes the last samples of all channels (ADC_N many)
 * as raw unsigned integers to the given location.
 * 0 represents GND, ADC_TOP represents AREF.
 * 
 * @param channels location for the samples to be written to
 */
void adc_getAll(uint16_t *channels);
/**
 * Writes the last samples of all channels
 * as floating point values to the given location.
 * 0 represents GND, 1 (included) represents AREF.
 * 
 * @param channels location for the samples to be written to
 */
void adc_getAllScaled(double *channels);



#endif /* ADC_H_ */
