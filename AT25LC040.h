/*
 * AT25LC040.h
 *
 * Created: 28.03.2018 21:06:37
 * Author: Gössl
 */


#ifndef AT25LC040_H_
#define AT25LC040_H_

//SPI_FREQUENCY 1MHz (max MHz/5V)
//SPI_MODE 0



#define AT25LC040_READ 0x03
#define AT25LC040_WRITE 0x02
#define AT25LC040_WRDI 0x04
#define AT25LC040_WREN 0x06
#define AT25LC040_RDSR 0x05
#define AT25LC040_WRSR 0x01



#include <avr/io.h>
#include <stdbool.h>



void AT25LC040_init(void);
uint8_t AT25LC040_readStatus(void);
void AT25LC040_writeEnable(void);
void AT25LC040_writeDisable(void);
void AT25LC040_write(uint16_t address, uint8_t data);
uint8_t AT25LC040_read(uint16_t address);

#endif /* AT25LC040_H_ */
