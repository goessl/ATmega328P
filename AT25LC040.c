/*
 * AT25LC040.c
 *
 * Created: 28.03.2018 21:06:37
 * Author: Gössl
 */


#include <util/delay.h>
#include "SPI.h"
#include "AT25LC040.h"



void AT25LC040_init(void)
{
	SPI_init();
}

uint8_t AT25LC040_readStatus(void)
{
	uint8_t status;
	
	SPI_selectSlave();
	_delay_ms(1);
	
	SPI_MasterTransmit(AT25LC040_RDSR);
	status = SPI_MasterTransmit(0x00);
	
	SPI_deselectSlave();
	_delay_ms(10);
	
	return status;
}

void AT25LC040_writeEnable(void)
{
	SPI_selectSlave();
	_delay_ms(1);
	
	SPI_MasterTransmit(AT25LC040_WREN);
	
	SPI_deselectSlave();
	_delay_ms(10);
}

void AT25LC040_writeDisable(void)
{
	SPI_selectSlave();
	_delay_ms(1);
	
	SPI_MasterTransmit(AT25LC040_WRDI);
	
	SPI_deselectSlave();
	_delay_ms(10);
}

void AT25LC040_write(uint16_t address, uint8_t data)
{
	AT25LC040_writeEnable();
	
	SPI_selectSlave();
	_delay_ms(1);
	
	SPI_MasterTransmit(AT25LC040_WRITE | (address & 0x100 >> 5));
	SPI_MasterTransmit((uint8_t)address);
	SPI_MasterTransmit(data);
	
	SPI_deselectSlave();
	_delay_ms(10);
}

uint8_t AT25LC040_read(uint16_t address)
{
	uint8_t data;
	
	SPI_selectSlave();
	_delay_ms(1);
	
	SPI_MasterTransmit(AT25LC040_READ | (address & 0x100 >> 5));
	SPI_MasterTransmit((uint8_t)address);
	data = SPI_MasterTransmit(0x00);
	
	SPI_deselectSlave();
	_delay_ms(10);
	
	return data;
}
