/*
 * NRF24L01.h
 * 
 * Author:      Sebastian Gössl
 * Hardware:    ATmega328P, NRF24L01+
 * 
 * LICENSE:
 * MIT License
 * 
 * Copyright (c) 2019 Sebastian Gössl
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



#ifndef NRF24L01_H_
#define NRF24L01_H_



#include <stdbool.h>
#include <stdint.h>



#ifndef NRF24L01_INTERRUPT
    #define NRF24L01_INTERRUPT 0
#endif

#define NRF24L01_MAX_PAYLOAD 32
#define NRF24L01_PIPES_N 6

#define NRF24L01_TX_STATUS_BUSY     0
#define NRF24L01_TX_STATUS_MAX_RT   1
#define NRF24L01_TX_STATUS_TX_DS    2
#define NRF24L01_TX_STATUS_TX_AP    3



//SPI: 10Mhz, Msb first, Mode 0
void NRF24L01_init(uint8_t* csnDDR, uint8_t* csnPORT, uint8_t csnPin, uint8_t* ceDDR, uint8_t* cePORT, uint8_t cePin);

void NRF24L01_sendData(uint8_t* data, uint8_t len);
uint8_t NRF24L01_getTXStatus(void);
void NRF24L01_setAcknowledgePayload(uint8_t* data, uint8_t len);

bool NRF24L01_dataAvailable(void);
uint8_t NRF24L01_getData(uint8_t* data);



#endif /* NRF24L01_H_ */
