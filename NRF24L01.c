/*
 * NRF24L01.c
 * 
 * Author:      Sebastian Gössl
 * Hardware:    ATmega328P, NRF24L01
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



//SPI_MODE 0
//SPI_DDORD 1
//SPI_FREQUENCY 8000000
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "SPI.h"
#include "NRF24L01.h"



#if NRF24L01_INTERRUPT == 0
    #define MPU6050_ISR INT0_vect
#elif NRF24L01_INTERRUPT == 1
    #define MPU6050_ISR INT1_vect
#else
    #error "No valid NRF24L01_INTERRUPT selected!"
#endif


#define NRF24L01_CMD_R_REGISTER(x)      (x & 0x1F)
#define NRF24L01_CMD_W_REGISTER(x)      ((x & 0x1F) | 0x20)
#define NRF24L01_CMD_R_RX_PAYLOAD       0x61
#define NRF24L01_CMD_W_TX_PAYLOAD       0xA0
#define NRF24L01_CMD_FLUSH_TX           0xE1
#define NRF24L01_CMD_FLUSH_RX           0xE2
#define NRF24L01_CMD_REUSE_TX_PL        0xE3
#define NRF24L01_CMD_R_RX_PL_WID        0x60
#define NRF24L01_CMD_W_ACK_PAYLOAD(p)   (0xA8 | (p & 0x7))
#define NRF24L01_CMD_W_TX_PAYLOAD_NOACK 0xB0
#define NRF24L01_CMD_ACTIVATE           0x50
#define NRF24L01_CMD_NOP                0xFF


#define NRF24L01_RA_CONFIG          0x00
#define NRF24L01_BIT_MASK_RX_DR     6
#define NRF24L01_BIT_MASK_TX_DS     5
#define NRF24L01_BIT_MASK_MAX_RT    4
#define NRF24L01_BIT_EN_CRC         3
#define NRF24L01_BIT_CRCO           2
#define NRF24L01_BIT_PWR_UP         1
#define NRF24L01_BIT_PRIM_RX        0

#define NRF24L01_RA_EN_AA           0x01
#define NRF24L01_BIT_ENAA_P5        5
#define NRF24L01_BIT_ENAA_P4        4
#define NRF24L01_BIT_ENAA_P3        3
#define NRF24L01_BIT_ENAA_P2        2
#define NRF24L01_BIT_ENAA_P1        1
#define NRF24L01_BIT_ENAA_P0        0

#define NRF24L01_RA_EN_RXADDR       0x02
#define NRF24L01_BIT_ERX_P5         5
#define NRF24L01_BIT_ERX_P4         4
#define NRF24L01_BIT_ERX_P3         3
#define NRF24L01_BIT_ERX_P2         2
#define NRF24L01_BIT_ERX_P1         1
#define NRF24L01_BIT_ERX_P0         0

#define NRF24L01_RA_SETUP_AW        0x03
#define NRF24L01_BIT_AW             0

#define NRF24L01_RA_SETUP_RETR      0x04
#define NRF24L01_BIT_ARD            4
#define NRF24L01_BIT_ARC            0

#define NRF24L01_RA_RF_CH           0x05

#define NRF24L01_RA_RF_SETUP        0x06
#define NRF24L01_BIT_RF_DR          3
#define NRF24L01_BIT_RF_PWR         1

#define NRF24L01_RA_STATUS          0x07
#define NRF24L01_BIT_RX_DR          6
#define NRF24L01_BIT_TX_DS          5
#define NRF24L01_BIT_MAX_RT         4
#define NRF24L01_BIT_RX_P_NO_MASK   0x0E
#define NRF24L01_BIT_STATUS_TX_FULL 0

#define NRF24L01_RA_OBSERVE_TX      0x08
#define NRF24L01_BIT_PLOS_CNT       4
#define NRF24L01_BIT_ARC_CNT        0

#define NRF24L01_RA_RPD             0x09
#define NRF24L01_BIT_RPD            0

#define NRF24L01_RA_RX_ADDR_P0      0x0A
#define NRF24L01_RA_RX_ADDR_P1      0x0B
#define NRF24L01_RA_RX_ADDR_P2      0x0C
#define NRF24L01_RA_RX_ADDR_P3      0x0D
#define NRF24L01_RA_RX_ADDR_P4      0x0E
#define NRF24L01_RA_RX_ADDR_P5      0x0F

#define NRF24L01_RA_TX_ADDR         0x10

#define NRF24L01_RA_RX_PWP0         0x11
#define NRF24L01_RA_RX_PWP1         0x12
#define NRF24L01_RA_RX_PWP2         0x13
#define NRF24L01_RA_RX_PWP3         0x14
#define NRF24L01_RA_RX_PWP4         0x15
#define NRF24L01_RA_RX_PWP5         0x16

#define NRF24L01_RA_FIFO_STATUS     0x17
#define NRF24L01_BIT_TX_REUSE       6
#define NRF24L01_BIT_FIFO_TX_FULL   5
#define NRF24L01_BIT_TX_EMPTY       4
#define NRF24L01_BIT_RX_FULL        1
#define NRF24L01_BIT_RX_EMPTY       0

#define NRF24L01_RA_DYNPD           0x1C
#define NRF24L01_BIT_DPL_P5         5
#define NRF24L01_BIT_DPL_P4         4
#define NRF24L01_BIT_DPL_P3         3
#define NRF24L01_BIT_DPL_P2         2
#define NRF24L01_BIT_DPL_P1         1
#define NRF24L01_BIT_DPL_P0         0

#define NRF24L01_RA_FEATURE         0x1D
#define NRF24L01_BIT_EN_DPL         2
#define NRF24L01_BIT_EN_ACK_PAY     1
#define NRF24L01_BIT_EN_DYN_ACK     0



static uint8_t NRF24L01_command(uint8_t cmd, uint8_t* data, size_t len);
static uint8_t NRF24L01_write(uint8_t address, uint8_t* data, size_t len);
static uint8_t NRF24L01_read(uint8_t address, uint8_t* data, size_t len);
static int NRF24L01_writeByteAndCheck(uint8_t address, uint8_t data);



static volatile uint8_t* NRF24L01_csnPORT;
static volatile uint8_t NRF24L01_csnPin;
static volatile uint8_t* NRF24L01_cePORT;
static volatile uint8_t NRF24L01_cePin;


static volatile int NRF24L01_primRX = 1;    //1: PRX, 0: PTX

static volatile int NRF24L01_txStatus = NRF24L01_SEND_STATUS_NOT_YET;

static volatile uint8_t NRF24L01_RXbuf[NRF24L01_MAX_PAYLOAD];
static volatile size_t NRF24L01_RXlen = 0;
static volatile size_t NRF24L01_RXpipe;



int NRF24L01_init(uint8_t* csnDDR, uint8_t* csnPORT, uint8_t csnPin, uint8_t* ceDDR, uint8_t* cePORT, uint8_t cePin)
{
    uint8_t data;
    
    
    
    //Outputs
    NRF24L01_csnPORT = csnPORT;
    NRF24L01_csnPin = csnPin;
    NRF24L01_cePORT = csnPORT;
    NRF24L01_cePin = cePin;
    
    *csnDDR |= (1 << NRF24L01_csnPin);
    *ceDDR |= (1 << NRF24L01_cePin);
    *NRF24L01_csnPORT |= (1 << NRF24L01_csnPin);
    *NRF24L01_cePORT &= ~(1 << NRF24L01_cePin);
    
    
    
    //Interrupt
    #if NRF24L01_INTERRUPT == 0
        //EICRA |= (1 << ISC01);
        EIMSK |= (1 << INT0);
    #elif NRF24L01_INTERRUPT == 1
        //EICRA |= (1 << ISC11);
        EIMSK |= (1 << INT1);
    #endif
    
    
    
    SPI_init();
    
    
    //Power on reset
    _delay_ms(100);
    
    
    
    //Flush FIFOs
    NRF24L01_command(NRF24L01_CMD_FLUSH_RX, NULL, 0);
    NRF24L01_command(NRF24L01_CMD_FLUSH_TX, NULL, 0);
    
    
    //Clear interrupts
    data = (1 << NRF24L01_BIT_RX_DR) | (1 << NRF24L01_BIT_MASK_TX_DS) | (1 << NRF24L01_BIT_MAX_RT);
    NRF24L01_write(NRF24L01_RA_STATUS, &data, 1);
    
    
    
    //Features
    data = (1 << NRF24L01_BIT_EN_DPL) | (1 << NRF24L01_BIT_EN_ACK_PAY);
    NRF24L01_write(NRF24L01_RA_FEATURE, &data, 1);
    NRF24L01_read(NRF24L01_RA_FEATURE, &data, 1);
    if(data == 0x00)
    {
        data = 0x73;
        NRF24L01_command(NRF24L01_CMD_ACTIVATE, &data, 1);
        
        data = (1 << NRF24L01_BIT_EN_DPL) | (1 << NRF24L01_BIT_EN_ACK_PAY);
        if(NRF24L01_writeByteAndCheck(NRF24L01_RA_EN_RXADDR, data))
            return 1;
    }
    
    
    //RX pipes
    data = (1 << NRF24L01_BIT_ERX_P0);
    if(NRF24L01_writeByteAndCheck(NRF24L01_RA_EN_RXADDR, data))
        return 1;
    
    //Dynamic payload
    data = (1 << NRF24L01_BIT_DPL_P0);
    if(NRF24L01_writeByteAndCheck(NRF24L01_RA_DYNPD, data))
        return 1;
    
    
    //Power up
    data = (1 << NRF24L01_BIT_EN_CRC) | (1 << NRF24L01_BIT_PWR_UP) | (1 << NRF24L01_BIT_PRIM_RX);
    if(NRF24L01_writeByteAndCheck(NRF24L01_RA_CONFIG, data))
        return 1;
    _delay_ms(2);
    
    
    //Enter RX mode
    *NRF24L01_cePORT |= (1 << NRF24L01_cePin);
    _delay_us(130);
    NRF24L01_primRX = 1;
    
    
    return 0;
}



static uint8_t NRF24L01_command(uint8_t cmd, uint8_t* data, size_t len)
{
    uint8_t status;
    
    
    
    *NRF24L01_csnPORT &= ~(1 << NRF24L01_csnPin);
    
    status = SPI_transmit(cmd);
    SPI_transmitBurst(data, data, len);
    
    *NRF24L01_csnPORT |= (1 << NRF24L01_csnPin);
    _delay_us(1);
    
    
    return status;
}

static uint8_t NRF24L01_write(uint8_t address, uint8_t* data, size_t len)
{
    return NRF24L01_command(NRF24L01_CMD_W_REGISTER(address), data, len);
}

static uint8_t NRF24L01_read(uint8_t address, uint8_t* data, size_t len)
{
    return NRF24L01_command(NRF24L01_CMD_R_REGISTER(address), data, len);
}

static int NRF24L01_writeByteAndCheck(uint8_t address, uint8_t data)
{
    uint8_t write = data, read;
    
    NRF24L01_write(address, &data, 1);
    NRF24L01_read(address, &read, 1);
    
    return (write != read);
}



int NRF24L01_send(uint8_t* data, size_t len)
{
    uint8_t byte;
    
    
    
    //Max payload length
    if(len > NRF24L01_MAX_PAYLOAD)
        return 1;
    
    //FIFO full
    NRF24L01_read(NRF24L01_RA_FIFO_STATUS, &byte, 1);
    if(byte & (1 << NRF24L01_BIT_FIFO_TX_FULL))
        return 1;
    
    
    
    //Write payload
    NRF24L01_command(NRF24L01_CMD_W_TX_PAYLOAD, data, len);
    
    
    
    //TX mode
    *NRF24L01_cePORT &= ~(1 << NRF24L01_cePin);
    byte = (1 << NRF24L01_BIT_EN_CRC) | (1 << NRF24L01_BIT_PWR_UP);
    NRF24L01_write(NRF24L01_RA_CONFIG, &byte, 1);
    
    *NRF24L01_cePORT |= (1 << NRF24L01_cePin);
    _delay_us(11);
    *NRF24L01_cePORT &= ~(1 << NRF24L01_cePin);
    _delay_us(130);
    
    NRF24L01_primRX = 0;
    NRF24L01_txStatus = NRF24L01_SEND_STATUS_NOT_YET;
    
    
    
    return 0;
}

uint8_t NRF24L01_sendStatus(void)
{
    return NRF24L01_txStatus;
}


size_t NRF24L01_dataAvailable(void)
{
    return NRF24L01_RXlen;
}

size_t NRF24L01_getData(uint8_t* data, size_t* pipe)
{
    size_t temp = NRF24L01_RXlen;
    
    memcpy(data, (uint8_t*)NRF24L01_RXbuf, NRF24L01_RXlen);
    *pipe = NRF24L01_RXpipe;
    
    NRF24L01_RXlen = 0;
    
    return temp;
}


int NRF24L01_writeAckPayload(uint8_t* data, size_t len)
{
    uint8_t byte;
    
    
    
    //Max payload length
    if(len > NRF24L01_MAX_PAYLOAD)
        return 1;
    
    //FIFO full
    NRF24L01_read(NRF24L01_RA_FIFO_STATUS, &byte, 1);
    if(byte & (1 << NRF24L01_BIT_FIFO_TX_FULL))
        return 1;
    
    
    
    //Write payload
    NRF24L01_command(NRF24L01_CMD_W_ACK_PAYLOAD(0), data, len);
    
    
    
    return 0;
}



ISR(MPU6050_ISR)
{
    uint8_t data, status = NRF24L01_command(NRF24L01_CMD_NOP, NULL, 0);
    
    
    
    //TX done
    if(status & ((1 << NRF24L01_BIT_TX_DS) | (1 << NRF24L01_BIT_MAX_RT)))
    {
        if(status & (1 << NRF24L01_BIT_TX_DS))
            NRF24L01_txStatus = NRF24L01_SEND_STATUS_DATA_SENT;
        else
        {
            NRF24L01_command(NRF24L01_CMD_FLUSH_TX, NULL, 0);
            NRF24L01_txStatus = NRF24L01_SEND_STATUS_MAX_RT;
        }
        
        
        if(!NRF24L01_primRX)
        {
            //RX mode
            data = (1 << NRF24L01_BIT_EN_CRC) | (1 << NRF24L01_BIT_PWR_UP) | (1 << NRF24L01_BIT_PRIM_RX);
            NRF24L01_write(NRF24L01_RA_CONFIG, &data, 1);
            *NRF24L01_cePORT |= (1 << NRF24L01_cePin);
            //_delay_us(130);
            NRF24L01_primRX = 1;
        }
    }
    
    
    
    //Data received
    if(status & (1 << NRF24L01_BIT_RX_DR))
    {
        NRF24L01_read(NRF24L01_RA_FIFO_STATUS, &data, 1);
        while(!(data & (1 << NRF24L01_BIT_RX_EMPTY)))
        {
            //Get pipe
            NRF24L01_RXpipe = (status & NRF24L01_BIT_RX_P_NO_MASK) >> 1;
            //Get payload length
            NRF24L01_RXlen = 0;
            NRF24L01_command(NRF24L01_CMD_R_RX_PL_WID, (uint8_t*)&NRF24L01_RXlen, 1);
            //Read payload
            NRF24L01_command(NRF24L01_CMD_R_RX_PAYLOAD, (uint8_t*)NRF24L01_RXbuf, NRF24L01_RXlen);
            
            NRF24L01_read(NRF24L01_RA_FIFO_STATUS, &data, 1);
        }
    }
    
    
    
    //Clear done interrupts
    NRF24L01_write(NRF24L01_RA_STATUS, &status, 1);
}
