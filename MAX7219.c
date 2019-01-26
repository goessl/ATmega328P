/*
 * MAX7219.c
 * 
 * Author:      Sebastian Gössl
 * Hardware:    ATmega328P, MAX7219
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
#include "SPI.h"
#include "MAX7219.h"



#define MAX7219_N 8

#define MAX7219_RA_NOOP         0x00
#define MAX7219_RA_DIGIT_0      0x01
#define MAX7219_RA_DIGIT_1      0x02
#define MAX7219_RA_DIGIT_2      0x03
#define MAX7219_RA_DIGIT_3      0x04
#define MAX7219_RA_DIGIT_4      0x05
#define MAX7219_RA_DIGIT_5      0x06
#define MAX7219_RA_DIGIT_6      0x07
#define MAX7219_RA_DIGIT_7      0x08
#define MAX7219_RA_DIGIT(x)     (x + MAX7219_RA_DIGIT_0)
#define MAX7219_RA_DECODE_MODE  0x09
#define MAX7219_RA_INTENSITY    0x0A
#define MAX7219_RA_SCAN_LIMIT   0x0B
#define MAX7219_RA_SHUTDOWN     0x0C
#define MAX7219_RA_DISPLAY_TEST 0x0B

#define MAX7219_FONT_MINUS      0x0A
#define MAX7219_FONT_E          0x0B
#define MAX7219_FONT_H          0x0C
#define MAX7219_FONT_L          0x0D
#define MAX7219_FONT_P          0x0E
#define MAX7219_FONT_BLANK      0x0F



static void MAX7219_write(uint8_t address, uint8_t data);



static uint8_t* MAX7219_ddr;
static uint8_t* MAX7219_port;
static size_t MAX7219_pin;



void MAX7219_init(uint8_t scanLimit, uint8_t* ddr, uint8_t* port, size_t pin)
{
    MAX7219_ddr = ddr;
    MAX7219_port = port;
    MAX7219_pin = pin;
    
    *MAX7219_ddr |= (1 << MAX7219_pin);
    *MAX7219_port |= (1 << MAX7219_pin);
    
    //SPI_init(100000, 0, 0, 0);
    
    MAX7219_write(MAX7219_RA_SHUTDOWN, 0x00);
    MAX7219_write(MAX7219_RA_DECODE_MODE, 0xFF);
    MAX7219_write(MAX7219_RA_INTENSITY, 0x0F);
    MAX7219_write(MAX7219_RA_SCAN_LIMIT, scanLimit);
    MAX7219_write(MAX7219_RA_SHUTDOWN, 0x01);
}



static void MAX7219_write(uint8_t address, uint8_t data)
{
    PORTB &= ~(1 << PORTB2);
    
    SPI_transmit(address);
    SPI_transmit(data);
    
    PORTB |= (1 << PORTB2);
}

static void MAX7219_reverse(uint8_t* data, size_t len)
{
    uint8_t t;
    size_t i, j;
    
    for(i=0, j=len-1; i<j; i++, j--)
        t = data[i], data[i] = data[j], data[j] = t;
}

static size_t MAX7219_intToDigitArray(int value, uint8_t* s)
{
    size_t i = 0;
    int sign;
    
    
    
    if((sign = value) < 0)
        value = -value;
    
    do
    {
        s[i++] = value % 10;
    }while(value /= 10);
    
    if(sign < 0)
        s[i++] = MAX7219_FONT_MINUS;
    
    
    MAX7219_reverse(s, i);
    
    
    return i;
}



void MAX7219_print(int value)
{
    uint8_t s[MAX7219_N + 1];
    size_t i, digits =  MAX7219_intToDigitArray(value, s);
    
    for(i=0; i<MAX7219_N; i++)
        MAX7219_write(MAX7219_RA_DIGIT(i), (i < digits)?s[i]:MAX7219_FONT_BLANK);
}

/*
static size_t MAX7219_intToDigitArray(int value, uint8_t* s)
{
    size_t i = 0;
    
    do
    {
        s[i++] = value % 10;
        value /= 10;
    }while(value);
    
    return i;
}



void MAX7219_print(int value)
{
    uint8_t s[MAX7219_N + 1];
    size_t i, digits =  MAX7219_intToDigitArray(value, s);
    
    for(i=0; i<MAX7219_N; i++)
        MAX7219_write(MAX7219_RA_DIGIT(i), (i < digits)?s[digits - i - 1]:MAX7219_FONT_BLANK);
}
*/
