/*
 * SSD1306.c
 * 
 * Author:      Sebastian Gössl
 * Hardware:    ATmega328P, SSD1306
 * 
 * Module for the SSD1306 driver.
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



#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "I2C.h"
#include "SSD1306.h"



// 0  4  8 12
// 1  5  9 13
// 2  6 10 14
// 3  7 11 15
static const uint8_t font[][SSD1306_FONT_HEIGHT*SSD1306_FONT_WIDTH / 8] =
{
    {0x9F, 0xF9},    //0
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},    //10
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},    //20
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},
    {0x9F, 0xF9},    //30
    {0x9F, 0xF9},
    {0x00, 0x00},    //SPACE
    {0xB0, 0x00},    //!
    {0x03, 0x03},    //"
    {0x7A, 0xFA},    //#
    {0xFA, 0x0D},    //$
    {0x49, 0x92},    //%
    {0x52, 0xEA},    //&
    {0x30, 0x00},    //'
    {0x60, 0x09},    //(
    {0x90, 0x06},    //)
    {0x25, 0x05},    //*
    {0xE4, 0x04},    //+
    {0xC0, 0x00},    //,
    {0x44, 0x04},    //-
    {0x80, 0x00},    //.
    {0xC0, 0x03},    ///
    {0x96, 0x06},    //0
    {0xFa, 0x08},    //1
    {0xD9, 0x0A},    //2
    {0xB9, 0x05},    //3
    {0x47, 0x4F},    //4
    {0xBB, 0x05},    //5
    {0xBE, 0x05},    //6
    {0xD1, 0x03},    //7
    {0xD2, 0x4B},    //8
    {0xDA, 0x06},    //9
    {0x50, 0x00},    //:
    {0xD0, 0x00},    //;
    {0x40, 0x0A},    //<
    {0xAA, 0x0A},    //=
    {0xA0, 0x04},    //>
    {0x90, 0x03},    //?
    {0x9F, 0x7F},    //@
    {0x5E, 0x0E},    //A
    {0xBF, 0x06},    //B
    {0x96, 0x09},    //C
    {0x9F, 0x06},    //D
    {0xBF, 0x09},    //E
    {0x5F, 0x01},    //F
    {0x96, 0x4D},    //G
    {0x2F, 0x0F},    //H
    {0xF9, 0x09},    //I
    {0x79, 0x01},    //J
    {0x2F, 0x0D},    //K
    {0x8F, 0x08},    //L
    {0x2F, 0xF1},    //M
    {0x2F, 0xF4},    //N
    {0x96, 0x69},    //O
    {0x5F, 0x02},    //P
    {0x96, 0xED},    //Q
    {0x5F, 0x0A},    //R
    {0xB0, 0x0D},    //S
    {0xF1, 0x01},    //T
    {0x87, 0x0F},    //U
    {0x87, 0x07},    //V
    {0x4F, 0xF4},    //W
    {0x69, 0x09},    //X
    {0xC3, 0x03},    //Y
    {0xD0, 0x0B},    //Z
    {0xF0, 0x09},    //[
    {0x30, 0x0C},    /*\*/
    {0x90, 0x0F},    //]
    {0x12, 0x02},    //^
    {0x88, 0x88},    //_
    {0x10, 0x02},    //`
    {0xA4, 0x0E},    //a
    {0xAF, 0x04},    //b
    {0xC0, 0x0A},    //c
    {0xA4, 0x0F},    //d
    {0xE4, 0x0A},    //e
    {0xE4, 0x05},    //f
    {0xD2, 0x0F},    //g
    {0x2F, 0x0C},    //h
    {0xD0, 0x00},    //i
    {0xD8, 0x00},    //j
    {0x4F, 0x0A},    //k
    {0x8F, 0x00},    //l
    {0x2E, 0xCE},    //m
    {0x2E, 0x0C},    //n
    {0xAC, 0x06},    //o
    {0xAE, 0x06},    //p
    {0xA6, 0x0E},    //q
    {0xE0, 0x02},    //r
    {0xE0, 0x0A},    //s
    {0xF2, 0x02},    //t
    {0x86, 0x0E},    //u
    {0x86, 0x06},    //v
    {0xC6, 0x6C},    //w
    {0x4A, 0x0A},    //x
    {0x4A, 0x02},    //y
    {0xA0, 0x0E},    //z
    {0x62, 0x09},    //{
    {0x69, 0x02},    //}
    {0xF0, 0x00},    //|
    {0x24, 0x24},    //~
    {0x9F, 0xF9},    //DEL
};



void SSD1306_init(void)
{
    I2C_init();
    
    
    //Display: Off
    SSD1306_sendCommand(SSD1306_DISPLAYOFF);
    
    //Enable charge pump
    SSD1306_sendCommand(SSD1306_CHARGEPUMP);
    SSD1306_sendCommand(0x14);
    
    //Memory mode: Vertical
    SSD1306_sendCommand(SSD1306_MEMORYMODE);
    SSD1306_sendCommand(0x01);
    
    //Segmentremap: Column 0 - SEG127
    SSD1306_sendCommand(SSD1306_SEGREMAP | 0x01);
    
    //COM output scan direction: Remapped mode
    SSD1306_sendCommand(SSD1306_COMSCANDEC);
    
    //Contrast: Max
    SSD1306_sendCommand(SSD1306_SETCONTRAST);
    SSD1306_sendCommand(0xFF);
    
    //Display: On
    SSD1306_sendCommand(SSD1306_DISPLAYON);
}


void SSD1306_sendCommand(uint8_t command)
{
    I2C_start();
    I2C_write(SSD1306_DEVICE_W);
    //Co: 0 is data only, D/C#: 0=data, 1=command
    I2C_write(0x00);
    I2C_write(command);
    I2C_stop();
}

void SSD1306_sendBuffer(uint8_t* buf)
{
    uint8_t packet, packet_byte;
    
    
    for(packet=0; packet<SSD1306_BUFFER_LENGTH/16; packet++)
    {
        I2C_start();
        I2C_write(SSD1306_DEVICE_W);
        I2C_write(0x40);
        for(packet_byte=0; packet_byte<16; packet_byte++)
            I2C_write(buf[packet*16 + packet_byte]);
        I2C_stop();
    }
}


void SSD1306_bufferClear(uint8_t* buf)
{
    memset(buf, 0x00, SSD1306_BUFFER_LENGTH);
}


void SSD1306_bufferClearPixel(uint8_t* buf, int x, int y)
{
    int bit = x*SSD1306_HEIGHT + y;
    
    buf[bit/8] &= ~(1 << (bit%8));
}

void SSD1306_bufferDrawPixel(uint8_t* buf, int x, int y)
{
    int bit = x*SSD1306_HEIGHT + y;
    
    buf[bit/8] |= (1 << (bit%8));
}


void SSD1306_bufferDrawLine(uint8_t* buf, int x1, int y1, int x2, int y2)
{
    //Bresenham's Line Algorithm
    
    //Current position
    int x = x1;
    int y = y1;
    //Absolute difference & step
    int dx = abs(x2 - x1);
    int sx = (x1<x2)?1:-1;
    
    int dy = -abs(y2 - y1);
    int sy = (y1<y2)?1:-1;
    //Error
    int err = dx + dy;
    int e2;
    
    
    while(x!=x2 || y != y2)
    {
        SSD1306_bufferDrawPixel(buf, x, y);
        
        e2 = 2 * err;
        
        if(e2 > dy)
        {
            err += dy;
            x += sx;
        }
        if(e2 < dx)
        {
            err += dx;
            y += sy;
        }
    }
}

void SSD1306_bufferDrawRect(uint8_t* buf, int x1, int y1, int x2, int y2)
{
    int i;
    
    for(i=x1; i<x2; i++)
    {
        SSD1306_bufferDrawPixel(buf, i, y1);
        SSD1306_bufferDrawPixel(buf, i, y2);
    }
    for(i=y1; i<y2; i++)
    {
        SSD1306_bufferDrawPixel(buf, x1, i);
        SSD1306_bufferDrawPixel(buf, x2, i);
    }
    SSD1306_bufferDrawPixel(buf, x2, y2);
}

void SSD1306_bufferDrawCircle(uint8_t* buf, int x, int y, int radius)
{
    //Bresenham Algorithm
    
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int xCurrent = 0;
    int yCurrent = radius;

    SSD1306_bufferDrawPixel(buf, x, y + radius);
    SSD1306_bufferDrawPixel(buf, x, y - radius);
    SSD1306_bufferDrawPixel(buf, x + radius, y);
    SSD1306_bufferDrawPixel(buf, x - radius, y);

    while(xCurrent < yCurrent)
    {
        if(f >= 0)
        {
            yCurrent--;
            ddF_y += 2;
            f += ddF_y;
        }
        xCurrent++;
        ddF_x += 2;
        f += ddF_x + 1;

        SSD1306_bufferDrawPixel(buf, x + xCurrent, y + yCurrent);
        SSD1306_bufferDrawPixel(buf, x - xCurrent, y + yCurrent);
        SSD1306_bufferDrawPixel(buf, x + xCurrent, y - yCurrent);
        SSD1306_bufferDrawPixel(buf, x - xCurrent, y - yCurrent);
        SSD1306_bufferDrawPixel(buf, x + yCurrent, y + xCurrent);
        SSD1306_bufferDrawPixel(buf, x - yCurrent, y + xCurrent);
        SSD1306_bufferDrawPixel(buf, x + yCurrent, y - xCurrent);
        SSD1306_bufferDrawPixel(buf, x - yCurrent, y - xCurrent);
    }
}


void SSD1306_bufferDrawCharacter(uint8_t* buf, int x, int y, char character)
{
    int i;
    
    for(i=0; i<SSD1306_FONT_HEIGHT*SSD1306_FONT_WIDTH; i++)
    {
        if(font[(int)character][i/8] & (1 << i%8))
            SSD1306_bufferDrawPixel(buf, x + i/SSD1306_FONT_HEIGHT, y + i%SSD1306_FONT_HEIGHT);
        else
            SSD1306_bufferClearPixel(buf, x + i/SSD1306_FONT_HEIGHT, y + i%SSD1306_FONT_HEIGHT);
    }
}

void SSD1306_bufferPrintString(uint8_t* buf, char* string)
{
    int i;
    int line=0, column=0;
    
    
    for(i=0; string[i]!='\0' && line<SSD1306_FONT_LINES; i++)
    {
        if(string[i] == '\n')
        {
            line++;
            column = 0;
        }
        else
        {
            SSD1306_bufferDrawCharacter(buf, column*(SSD1306_FONT_WIDTH+1) + 1, line*(SSD1306_FONT_HEIGHT+1) + 1, string[i]);
            column++;
            
            if(column >= SSD1306_FONT_LINE_LENGTH)
            {
                line++;
                column = 0;
            }
        }
    }
}


void SSD1306_print(char* string)
{
    uint8_t buf[SSD1306_BUFFER_LENGTH];
    
    
    SSD1306_bufferClear(buf);
    SSD1306_bufferPrintString(buf, string);
    SSD1306_sendBuffer(buf);
}

int SSD1306_printf(const char *fmt, ...)
{
    int r;
    va_list args;
    char s[SSD1306_FONT_MAX_CHARACTERS];
    uint8_t buf[SSD1306_BUFFER_LENGTH];
    
    
    
    va_start(args, fmt);
    r = vsnprintf(s, SSD1306_FONT_MAX_CHARACTERS, fmt, args);
    va_end(args);
    
    SSD1306_bufferClear(buf);
    SSD1306_bufferPrintString(buf, s);
    SSD1306_sendBuffer(buf);
    
    return r;
}
