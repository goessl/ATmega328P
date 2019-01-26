/*
 * SSD1306.h
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



#ifndef SSD1306_H_
#define SSD1306_H_



#define SSD1306_HEIGHT               64
#define SSD1306_WIDTH                128
#define SSD1306_PIXEL                (SSD1306_HEIGHT * SSD1306_WIDTH)
#define SSD1306_BUFFER_LENGTH        (SSD1306_PIXEL / 8)


#define SSD1306_DEVICE              0x78
#define SSD1306_DEVICE_R            (SSD1306_DEVICE | 0x01)
#define SSD1306_DEVICE_W            (SSD1306_DEVICE & ~0x01)

#define SSD1306_SETCONTRAST         0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON        0xA5
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_DISPLAYON           0xAF
#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETVCOMDETECT       0xDB
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETMULTIPLEX        0xA8
#define SSD1306_SETLOWCOLUMN        0x00
#define SSD1306_SETHIGHCOLUMN       0x10
#define SSD1306_SETSTARTLINE        0x40
#define SSD1306_MEMORYMODE          0x20
#define SSD1306_COLUMNADDR          0x21
#define SSD1306_PAGEADDR            0x22
#define SSD1306_COMSCANINC          0xC0
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_SEGREMAP            0xA0
#define SSD1306_CHARGEPUMP          0x8D
#define SSD1306_SWITCHCAPVCC        0x2
#define SSD1306_NOP                 0xE3

//Font
#define SSD1306_FONT_HEIGHT         4
#define SSD1306_FONT_WIDTH          4
#define SSD1306_FONT_LINE_LENGTH    (SSD1306_WIDTH / (SSD1306_FONT_WIDTH + 1))
#define SSD1306_FONT_LINES          (SSD1306_HEIGHT / (SSD1306_FONT_HEIGHT + 1))
#define SSD1306_FONT_MAX_CHARACTERS (SSD1306_FONT_LINE_LENGTH * SSD1306_FONT_LINES)



void SSD1306_init();

void SSD1306_sendCommand(uint8_t command);
void SSD1306_sendBuffer(uint8_t* buf);


void SSD1306_bufferClear(uint8_t* buf);

void SSD1306_bufferClearPixel(uint8_t* buf, int x, int y);
void SSD1306_bufferDrawPixel(uint8_t* buf, int x, int y);

void SSD1306_bufferDrawLine(uint8_t* buf, int x1, int y1, int x2, int y2);
void SSD1306_bufferDrawRect(uint8_t* buf, int x1, int y1, int x2, int y2);
void SSD1306_bufferDrawCircle(uint8_t* buf, int x, int y, int radius);

void SSD1306_bufferDrawCharacter(uint8_t* buf, int x, int y, char character);
void SSD1306_bufferPrintString(uint8_t* buf, char* string);


void SSD1306_print(char* string);
int SSD1306_printf(const char *fmt, ...);



#endif /* SSD1306_H_ */
