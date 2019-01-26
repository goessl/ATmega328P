/*
 * ADXL345_I2C.h
 *
 * Created: 28.02.2018 11:20:12
 * Author: Gössl
 */



#ifndef ADXL345_I2C_H_
#define ADXL345_I2C_H_



#define ADXL345_I2C_DEVICE			0xA6		//Reset Device Address for ADXL345
#define ADXL345_I2C_DEVICE_W		(ADXL345_I2C_DEVICE & ~0x01)
#define ADXL345_I2C_DEVICE_R		(ADXL345_I2C_DEVICE | 0x01)

#define ADXL345_I2C_DEVID			0x00		//Device ID
#define ADXL345_I2C_THRESH_TAP		0x1D		//Tap threshold
#define ADXL345_I2C_OFSX			0x1E		//X-axis offset
#define ADXL345_I2C_OFSY			0x1F		//Y-axis offset
#define ADXL345_I2C_OFSZ			0x20		//Z-axis offset
#define ADXL345_I2C_DUR				0x21		//Tap duration
#define ADXL345_I2C_LATENT			0x22		//Tap latency
#define ADXL345_I2C_WINDOW			0x23		//Tap window
#define ADXL345_I2C_THRESH_ACT		0x24		//Activity threshold
#define ADXL345_I2C_THRESH_INACT	0x25		//Inactivity threshold
#define ADXL345_I2C_TIME_INACT		0x26		//Inactivity time
#define ADXL345_I2C_ACT_INACT_CTL	0x27		//Axis enable control for activity and inactivity detection
#define ADXL345_I2C_THRESH_FF		0x28		//Free-fall threshold
#define ADXL345_I2C_TIME_FF			0x29		//Free-fall time
#define ADXL345_I2C_TAP_AXES		0x2A		//Axis control for tap/double tap
#define ADXL345_I2C_ACT_TAP_STATUS	0x2B		//Source of tap/double tap
#define ADXL345_I2C_BW_RATE			0x2C		//Data rate and power mode control
#define ADXL345_I2C_POWER_CTL		0x2D		//Power-Saving Features Control
#define ADXL345_I2C_INT_ENABLE		0x2E		//Interrupt enable control
#define ADXL345_I2C_INT_MAP			0x2F		//Interrupt mapping control
#define ADXL345_I2C_INT_SOURCE		0x30		//Source of interrupts
#define ADXL345_I2C_DATA_FORMAT		0x31		//Data format control
#define ADXL345_I2C_DATAX0			0x32		//X-Axis Data 0
#define ADXL345_I2C_DATAX1			0x33		//X-Axis Data 1
#define ADXL345_I2C_DATAY0			0x34		//Y-Axis Data 0
#define ADXL345_I2C_DATAY1			0x35		//Y-Axis Data 1
#define ADXL345_I2C_DATAZ0			0x36		//Z-Axis Data 0
#define ADXL345_I2C_DATAZ1			0x37		//Z-Axis Data 1
#define ADXL345_I2C_FIFO_CTL		0x38		//FIFO control
#define ADXL345_I2C_FIFO_STATUS		0x39		//FIFO status


#define ADXL345_I2C_RANGE			2			//Range setting (2, 4, 8, or 16g; default is 2)
#define ADXL345_I2C_SCALE			(2.0 / 0x3FF * ADXL345_I2C_RANGE)



#include <avr/io.h>



void ADXL345_I2C_init(void);

void ADXL345_I2C_write(uint8_t address, uint8_t data);
void ADXL345_I2C_writeBuffer(uint8_t address, uint8_t buf[], int len);

uint8_t ADXL345_I2C_read(uint8_t address);
void ADXL345_I2C_readBuffer(uint8_t address, uint8_t* buf, int len);

void ADXL345_I2C_readAccel(int16_t* x, int16_t* y, int16_t* z);
void ADXL345_I2C_readAccelScaled(double* x, double* y, double* z);



#endif
