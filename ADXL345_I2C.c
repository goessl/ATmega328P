/*
 * ADXL345_I2C.c
 *
 * Created: 28.02.2018 11:20:12
 * Author: Gössl
 */



#include "I2C.h"
#include "ADXL345_I2C.h"



void ADXL345_I2C_init(void)
{
	I2C_init();
	
	//Autosleep & Measure (non standby) mode
	ADXL345_I2C_write(ADXL345_I2C_POWER_CTL, (1<<3) | (1<<4));
	
	//Range settings
	#if (ADXL345_I2C_RANGE == 4)
		ADXL345_I2C_write(ADXL345_I2C_DATA_FORMAT, 0x01);
	#elif (ADXL345_I2C_RANGE == 8)
		ADXL345_I2C_write(ADXL345_I2C_DATA_FORMAT, 0x02);
	#elif (ADXL345_I2C_RANGE == 16)
		ADXL345_I2C_write(ADXL345_I2C_DATA_FORMAT, 0x03);
	#else
		ADXL345_I2C_write(ADXL345_I2C_DATA_FORMAT, 0x00);
	#endif
}


void ADXL345_I2C_write(uint8_t address, uint8_t data)
{
	I2C_start();
	I2C_write(ADXL345_I2C_DEVICE_W);
	I2C_write(address);
	I2C_write(data);
	I2C_stop();
}

void ADXL345_I2C_writeMulti(uint8_t address, uint8_t buf[], int len)
{
	I2C_start();
	I2C_write(ADXL345_I2C_DEVICE_W);
	I2C_write(address);
	I2C_writeBuffer(buf, len);
	I2C_stop();
}


uint8_t ADXL345_I2C_read(uint8_t address)
{
	uint8_t data;
	
	I2C_start();
	I2C_write(ADXL345_I2C_DEVICE_W);
	I2C_write(address);
	I2C_stop();
	
	I2C_start();
	I2C_write(ADXL345_I2C_DEVICE_R);
	data = I2C_readNoAck();
	I2C_stop();
	
	return data;
}

void ADXL345_I2C_readMulti(uint8_t address, uint8_t buf[], int len)
{
	I2C_start();
	I2C_write(ADXL345_I2C_DEVICE_W);
	I2C_write(address);
	I2C_stop();
	
	I2C_start();
	I2C_write(ADXL345_I2C_DEVICE_R);
	I2C_readAckBuffer(buf, len-1);
	buf[len-1] = I2C_readNoAck();
	I2C_stop();
}


void ADXL345_I2C_readAccel(int16_t* x, int16_t* y, int16_t* z)
{
	uint8_t buf[6];
	
	ADXL345_I2C_readMulti(ADXL345_I2C_DATAX0, buf, 6);
	
	*x = (buf[1] << 8) | buf[0];
	*y = (buf[3] << 8) | buf[2];
	*z = (buf[5] << 8) | buf[4];
}

void ADXL345_I2C_readAccelScaled(double* x, double* y, double* z)
{
	int16_t xRaw, yRaw, zRaw;
	
	ADXL345_I2C_readAccel(&xRaw, &yRaw, &zRaw);
	
	*x = xRaw * ADXL345_I2C_SCALE;
	*y = yRaw * ADXL345_I2C_SCALE;
	*z = zRaw * ADXL345_I2C_SCALE;
}
