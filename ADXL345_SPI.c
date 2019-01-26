/*
 * ADXL345.c
 *
 * Created: 05.12.2017 07:30:50
 * Author: Gössl
 */


#include <avr/io.h>
#include "SPI.h"
#include "ADXL345_SPI.h"



void ADXL345_SPI_init(void)
{
	SPI_init();
	
	ADXL345_SPI_write(ADXL345_SPI_POWER_CTL, 0x00);
	ADXL345_SPI_write(ADXL345_SPI_POWER_CTL, (1 << 4));
	ADXL345_SPI_write(ADXL345_SPI_POWER_CTL, (1 << 3));
	
	ADXL345_SPI_write(ADXL345_SPI_DATA_FORMAT, 0x00);
}

void ADXL345_SPI_write(uint8_t address, uint8_t data)
{
	uint8_t _address = ADXL345_SPI_ADDRESS_W_SB(address);
	
	SPI_selectSlave();
	SPI_MasterTransmit(_address);
	SPI_MasterTransmit(data);
	SPI_deselectSlave();
}

void ADXL345_SPI_writeMulti(uint8_t address, uint8_t buf[], int buflen)
{
	int i;
	uint8_t _address = ADXL345_SPI_ADDRESS_W_MB(address);
	
	SPI_selectSlave();
	SPI_MasterTransmit(_address);
	for(i=0; i<buflen; i++)
		SPI_MasterTransmit(buf[i]);
	SPI_deselectSlave();
}

uint8_t ADXL345_SPI_read(uint8_t address)
{
	uint8_t response;
	char _address = ADXL345_SPI_ADDRESS_R_SB(address);
	
	SPI_selectSlave();
	SPI_MasterTransmit(_address);
	response = SPI_MasterTransmit(0x00);
	SPI_deselectSlave();
	
	return response;
}

void ADXL345_SPI_readMulti(uint8_t address, uint8_t buf[], int buflen)
{
	char _address = ADXL345_SPI_ADDRESS_R_MB(address);

	SPI_selectSlave();
	SPI_MasterTransmit(_address);
	for(int i=0; i<buflen; i++)
		buf[i] = SPI_MasterTransmit(0x00);
	SPI_deselectSlave();
}

void ADXL345_SPI_readAccel(int16_t* x, int16_t* y, int16_t* z)
{
	uint8_t buf[6];

	ADXL345_SPI_readMulti(ADXL345_SPI_DATAX0, buf, 6);
	
	*x = (buf[1] << 8) | buf[0];
	*y = (buf[3] << 8) | buf[2];
	*z = (buf[5] << 8) | buf[4];
}
