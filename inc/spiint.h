/*
 * spiint.h
 * 
 * Buffered, interrupt based SPI driver.
 * 
 * Author:      Sebastian Goessl
 * Hardware:    ATmega328P
 * 
 * LICENSE:
 * MIT License
 * 
 * Copyright (c) 2019 Sebastian Goessl
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */



#ifndef SPIINT_H_
#define SPIINT_H_



#include <stdbool.h>    //bool type
#include <stddef.h>     //size_t type
#include <stdint.h>     //uint8_t type



//default to 1MHz
#ifndef SPI_FREQUENCY
    #define SPI_FREQUENCY 1000000
#endif

//set lower limit if needed
#ifndef SPI_MIN_FREQUENCY
    #define SPI_MIN_FREQUENCY 0
#endif

//datasheet table 19-2
//(CPOL << 1) || (CPHA << 0)
#ifndef SPI_MODE
    #define SPI_MODE 0
#endif

//0: MSB first, 1: LSB first
#ifndef SPI_DORD
    #define SPI_DORD 0
#endif



/**
 * Initializes the SPI peripherals to operate at SPI_FREQUENCY
 * (maximum and optionally SPI_MIN_FREQUENCY minimum)
 * and in SPI_MODE and in SPI_DORD.
 * Configures SS, MOSI & SCK (PB2, PB3 & PB5) as outputs
 * and MISO (PB4) as input.
 * SS (PB2) has to be kept as output so that the SPI hardware
 * doesn't switch into slave mode.
 * Interrupts have to be enabled (by calling avr/interrupt.h's sei).
 */
void spiint_init(void);

/**
 * Returns true if currently a transmission is ongoing.
 * 
 * @return if currently a transmission is ongoing
 */
bool spiint_isBusy(void);
/**
 * Blocks until the transmission is completed.
 */
void spiint_flush(void);

/**
 * Starts shifting out multiple bytes while simultaneously shifting bytes in.
 * Pulls the SS pin low (if a PORT register is provided,
 * if NULL is given, no SS actions will be taken) and starts
 * shifting in and out bytes over the SPI.
 * This function then returns while the transmission continues,
 * getting handled by interrupts routines.
 * The data locations (out and in) must not be changed
 * while the transmission is ongoing as the bytes are read/written as
 * they are needed.
 * When all bytes are shifted out/in the SS pin (if port not NULL)
 * is set high again.
 * When an transmission is still ongoing, this function blocks until the
 * transmission is completed before starting a new one.
 * 
 * @param out location of the bytes to shift out
 * @param in location where the shifted in bytes should be written to
 * @param len number of bytes to shift out/in
 * @param port address of the PORT register the SS is connected to
 * or null in no SS signal should be used
 * @param pin number of the corresponding bit (0-7) in the PORT register
 * the SS i connected to
 */
void spiint_transmitBurst(uint8_t *out, uint8_t *in, size_t len,
    uint8_t *port, uint8_t pin);



#endif /* SPIINT_H_ */
