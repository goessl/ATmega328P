/*
 * ADXL345.h
 *
 * Created: 05.12.2017 07:27:08
 * Author: Gössl
 */


#ifndef ADXL345_SPI_H_
#define ADXL345_SPI_H_

//SPI_FREQUENCY 1MHz (max 5MHz)
//SPI_MODE 3



#define ADXL345_SPI_ADDRESS_W(a)	((a) & ~(1 << 7))
#define ADXL345_SPI_ADDRESS_R(a)	((a) | (1 << 7))
#define ADXL345_SPI_ADDRESS_SB(a)	((a) & ~(1 << 6))
#define ADXL345_SPI_ADDRESS_MB(a)	((a) | (1 << 6))

#define ADXL345_SPI_ADDRESS_W_SB(a)	(ADXL345_SPI_ADDRESS_SB(ADXL345_SPI_ADDRESS_W(a)))
#define ADXL345_SPI_ADDRESS_W_MB(a)	(ADXL345_SPI_ADDRESS_MB(ADXL345_SPI_ADDRESS_W(a)))
#define ADXL345_SPI_ADDRESS_R_SB(a)	(ADXL345_SPI_ADDRESS_SB(ADXL345_SPI_ADDRESS_R(a)))
#define ADXL345_SPI_ADDRESS_R_MB(a)	(ADXL345_SPI_ADDRESS_MB(ADXL345_SPI_ADDRESS_R(a)))

#define ADXL345_SPI_POWER_CTL		0x2D		// Power-Saving Features Control
#define ADXL345_SPI_DATA_FORMAT		0x31		// Data Format Control
#define ADXL345_SPI_DATAX0			0x32		// X-Axis Data 0
#define ADXL345_SPI_DATAX1			0x33		// X-Axis Data 1
#define ADXL345_SPI_DATAY0			0x34		// Y-Axis Data 0
#define ADXL345_SPI_DATAY1			0x35		// Y-Axis Data 1
#define ADXL345_SPI_DATAZ0			0x36		// Z-Axis Data 0
#define ADXL345_SPI_DATAZ1			0x37		// Z-Axis Data 1



#include <avr/io.h>



void ADXL345_SPI_init(void);
void ADXL345_SPI_write(uint8_t address, uint8_t data);
void ADXL345_SPI_writeMulti(uint8_t address, uint8_t buf[], int buflen);
uint8_t ADXL345_SPI_read(uint8_t address);
void ADXL345_SPI_readMulti(uint8_t address, uint8_t buf[], int buflen);
void ADXL345_SPI_readAccel(int16_t* x, int16_t* y, int16_t* z);

#endif /* ADXL345_H_ */
