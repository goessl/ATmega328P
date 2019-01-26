/*
 * SSD1306.h
 *
 * Created: 04.04.2018 09:31:48
 * Author: Gössl
 */



#ifndef SSD1306_H_
#define SSD1306_H_



#define SSD1306_HEIGHT				64
#define SSD1306_WIDTH				128
#define SSD1306_PIXEL				(SSD1306_HEIGHT * SSD1306_WIDTH)
#define SSD1306_BUFFER_LENGTH		(SSD1306_PIXEL / 8)


#define SSD1306_DEVICE				0x78
#define SSD1306_DEVICE_R			(SSD1306_DEVICE | 0x01)
#define SSD1306_DEVICE_W			(SSD1306_DEVICE & ~0x01)

#define SSD1306_SETCONTRAST			0x81
#define SSD1306_DISPLAYALLON_RESUME	0xA4
#define SSD1306_DISPLAYALLON		0xA5
#define SSD1306_NORMALDISPLAY		0xA6
#define SSD1306_INVERTDISPLAY		0xA7
#define SSD1306_DISPLAYOFF			0xAE
#define SSD1306_DISPLAYON			0xAF
#define SSD1306_SETDISPLAYOFFSET	0xD3
#define SSD1306_SETCOMPINS			0xDA
#define SSD1306_SETVCOMDETECT		0xDB
#define SSD1306_SETDISPLAYCLOCKDIV	0xD5
#define SSD1306_SETPRECHARGE		0xD9
#define SSD1306_SETMULTIPLEX		0xA8
#define SSD1306_SETLOWCOLUMN		0x00
#define SSD1306_SETHIGHCOLUMN		0x10
#define SSD1306_SETSTARTLINE		0x40
#define SSD1306_MEMORYMODE			0x20
#define SSD1306_COLUMNADDR			0x21
#define SSD1306_PAGEADDR			0x22
#define SSD1306_COMSCANINC			0xC0
#define SSD1306_COMSCANDEC			0xC8
#define SSD1306_SEGREMAP			0xA0
#define SSD1306_CHARGEPUMP			0x8D
#define SSD1306_SWITCHCAPVCC		0x2
#define SSD1306_NOP					0xE3

//Font
#define SSD1306_FONT_HEIGHT			4
#define SSD1306_FONT_WIDTH			4
#define SSD1306_FONT_LINE_LENGTH	(SSD1306_WIDTH / (SSD1306_FONT_WIDTH + 1))
#define SSD1306_FONT_LINES			(SSD1306_HEIGHT / (SSD1306_FONT_HEIGHT + 1))



void SSD1306_init();

void SSD1306_sendCommand(uint8_t command);
void SSD1306_sendBuffer(uint8_t* buffer);

void SSD1306_bufferClear(uint8_t buf[]);
void SSD1306_bufferClearPixel(int x, int y, uint8_t buf[]);
void SSD1306_bufferDrawPixel(int x, int y, uint8_t buf[]);
void SSD1306_bufferDrawCharacter(int x, int y, char character, uint8_t buf[]);
void SSD1306_bufferPrintString(char string[], uint8_t buf[]);



#endif /* SSD1306_H_ */
