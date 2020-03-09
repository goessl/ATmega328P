/*
 * NRF24L01.c
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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */



#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "SPI.h"
#include "NRF24L01.h"



//Commands
#define NRF24L01_CMD_R_REGISTER(x)      (x & 0x1F)
#define NRF24L01_CMD_W_REGISTER(x)      ((x & 0x1F) | 0x20)
#define NRF24L01_CMD_R_RX_PAYLOAD       0x61
#define NRF24L01_CMD_W_TX_PAYLOAD       0xA0
#define NRF24L01_CMD_FLUSH_TX           0xE1
#define NRF24L01_CMD_FLUSH_RX           0xE2
#define NRF24L01_CMD_REUSE_TX_PL        0xE3
#define NRF24L01_CMD_R_RX_PL_WID        0x60
#define NRF24L01_CMD_W_ACK_PAYLOAD(p)   (0xA8 | (p & 0x07))
#define NRF24L01_CMD_W_TX_PAYLOAD_NOACK 0xB0
#define NRF24L01_CMD_ACTIVATE           0x50
#define NRF24L01_CMD_ACTIVATE_DATA      0x73
#define NRF24L01_CMD_NOP                0xFF

//Registers
//Config
#define NRF24L01_RA_CONFIG          0x00
#define NRF24L01_BIT_MASK_RX_DR     6
#define NRF24L01_BIT_MASK_TX_DS     5
#define NRF24L01_BIT_MASK_MAX_RT    4
#define NRF24L01_BIT_EN_CRC         3
#define NRF24L01_BIT_CRCO           2
#define NRF24L01_BIT_PWR_UP         1
#define NRF24L01_BIT_PRIM_RX        0
//Enable auto acknowledge
#define NRF24L01_RA_EN_AA           0x01
#define NRF24L01_BIT_ENAA_P5        5
#define NRF24L01_BIT_ENAA_P4        4
#define NRF24L01_BIT_ENAA_P3        3
#define NRF24L01_BIT_ENAA_P2        2
#define NRF24L01_BIT_ENAA_P1        1
#define NRF24L01_BIT_ENAA_P0        0
#define NRF24L01_BIT_ENAA_P(pipe)   (NRF24L01_BIT_ENAA_P0 + pipe)
//Enable RX pipe
#define NRF24L01_RA_EN_RXADDR       0x02
#define NRF24L01_BIT_ERX_P5         5
#define NRF24L01_BIT_ERX_P4         4
#define NRF24L01_BIT_ERX_P3         3
#define NRF24L01_BIT_ERX_P2         2
#define NRF24L01_BIT_ERX_P1         1
#define NRF24L01_BIT_ERX_P0         0
#define NRF24L01_BIT_ERX_P(pipe)    (NRF24L01_BIT_ERX_P0 + pipe)
//Setup address width
#define NRF24L01_RA_SETUP_AW        0x03
#define NRF24L01_BIT_AW             0
#define NRF24L01_LEN_AW             2
//Setup retries
#define NRF24L01_RA_SETUP_RETR      0x04
#define NRF24L01_BIT_ARD            4
#define NRF24L01_LEN_ARD            4
#define NRF24L01_BIT_ARC            0
#define NRF24L01_LEN_ARC            4
//RF Channel
#define NRF24L01_RA_RF_CH           0x05
#define NRF24L01_BIT_RF_CH          0
#define NRF24L01_LEN_RF_CH          7
//RF Setup
#define NRF24L01_RA_RF_SETUP        0x06
#define NRF24L01_BIT_CONT_WAVE      7
#define NRF24L01_BIT_RF_DR_LOW      5
#define NRF24L01_BIT_PLL_LOCK       4
#define NRF24L01_BIT_RF_DR_HIGH     3
#define NRF24L01_BIT_RF_PWR         1
#define NRF24L01_LEN_RF_PWR         2
//Status
#define NRF24L01_RA_STATUS          0x07
#define NRF24L01_BIT_RX_DR          6
#define NRF24L01_BIT_TX_DS          5
#define NRF24L01_BIT_MAX_RT         4
#define NRF24L01_BIT_RX_P_NO        1
#define NRF24L01_LEN_RX_P_NO        3
#define NRF24L01_BIT_STATUS_TX_FULL 0
//Observe TX
#define NRF24L01_RA_OBSERVE_TX      0x08
#define NRF24L01_BIT_PLOS_CNT       4
#define NRF24L01_LEN_PLOS_CNT       4
#define NRF24L01_BIT_ARC_CNT        0
#define NRF24L01_LEN_ARC_CNT        4
//Received power detector
#define NRF24L01_RA_RPD             0x09
#define NRF24L01_BIT_RPD            0
//RX addresses
#define NRF24L01_RA_RX_ADDR_P0      0x0A
#define NRF24L01_RA_RX_ADDR_P1      0x0B
#define NRF24L01_RA_RX_ADDR_P2      0x0C
#define NRF24L01_RA_RX_ADDR_P3      0x0D
#define NRF24L01_RA_RX_ADDR_P4      0x0E
#define NRF24L01_RA_RX_ADDR_P5      0x0F
#define NRF24L01_RA_RX_ADDR_P(pipe) (NRF24L01_RA_RX_ADDR_P0 + pipe)
#define NRF24L01_RA_RX_ADDR_P0_LEN  5
#define NRF24L01_RA_RX_ADDR_P1_LEN  5
#define NRF24L01_RA_RX_ADDR_P2_LEN  1
#define NRF24L01_RA_RX_ADDR_P3_LEN  1
#define NRF24L01_RA_RX_ADDR_P4_LEN  1
#define NRF24L01_RA_RX_ADDR_P4_LEN  1
#define NRF24L01_RA_RX_ADDR_P_LEN(pipe) ((pipe == 0 || pipe == 1)?5:1)
//TX address
#define NRF24L01_RA_TX_ADDR         0x10
#define NRF24L01_RA_TX_ADDR_LEN     5
//Payload widths
#define NRF24L01_RA_RX_PW_P0        0x11
#define NRF24L01_RA_RX_PW_P1        0x12
#define NRF24L01_RA_RX_PW_P2        0x13
#define NRF24L01_RA_RX_PW_P3        0x14
#define NRF24L01_RA_RX_PW_P4        0x15
#define NRF24L01_RA_RX_PW_P5        0x16
#define NRF24L01_RA_RX_PW_P(pipe)   (NRF24L01_RA_RX_PW_P0 + pipe)
#define NRF24L01_BIT_RX_PW_P        0
#define NRF24L01_LEN_RX_PW_P        6
//FIFO status
#define NRF24L01_RA_FIFO_STATUS     0x17
#define NRF24L01_BIT_TX_REUSE       6
#define NRF24L01_BIT_FIFO_TX_FULL   5
#define NRF24L01_BIT_TX_EMPTY       4
#define NRF24L01_BIT_RX_FULL        1
#define NRF24L01_BIT_RX_EMPTY       0
//Dynamic payload
#define NRF24L01_RA_DYNPD           0x1C
#define NRF24L01_BIT_DPL_P5         5
#define NRF24L01_BIT_DPL_P4         4
#define NRF24L01_BIT_DPL_P3         3
#define NRF24L01_BIT_DPL_P2         2
#define NRF24L01_BIT_DPL_P1         1
#define NRF24L01_BIT_DPL_P0         0
#define NRF24L01_BIT_DPL_P(pipe)    (NRF24L01_BIT_DPL_P0 + pipe)
//Feature
#define NRF24L01_RA_FEATURE         0x1D
#define NRF24L01_BIT_EN_DPL         2
#define NRF24L01_BIT_EN_ACK_PAY     1
#define NRF24L01_BIT_EN_DYN_ACK     0



static volatile uint8_t* NRF24L01_csnPORT;
static volatile uint8_t NRF24L01_csnPin;
static volatile uint8_t* NRF24L01_cePORT;
static volatile uint8_t NRF24L01_cePin;



static void NRF24L01_setCSN(bool value)
{
    if(value)
        *NRF24L01_csnPORT |= (1 << NRF24L01_csnPin);
    else
        *NRF24L01_csnPORT &= ~(1 << NRF24L01_csnPin);
}

static void NRF24L01_setCE(bool value)
{
    if(value)
        *NRF24L01_cePORT |= (1 << NRF24L01_cePin);
    else
        *NRF24L01_cePORT &= ~(1 << NRF24L01_cePin);
}


//Commands
static uint8_t NRF24L01_command(uint8_t cmd, uint8_t* data, size_t len)
{
    uint8_t status;
    
    
    NRF24L01_setCSN(false);
    
    status = SPI_transmit(cmd);
    SPI_transmitBurst(data, data, len);
    
    NRF24L01_setCSN(true);
    
    
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

static uint8_t NRF24L01_writeByte(uint8_t address, uint8_t data)
{
    return NRF24L01_write(address, &data, 1);
}

static uint8_t NRF24L01_readByte(uint8_t address)
{
    uint8_t data;
    
    NRF24L01_read(address, &data, 1);
    
    return data;
}

static uint8_t NRF24L01_writeBits(uint8_t address, uint8_t start, uint8_t len,
    uint8_t data)
{
    uint8_t mask = (1 << len) - 1;
    uint8_t temp = NRF24L01_readByte(address);
    
    temp &= ~(mask << start);
    temp |= (data & mask) << start;
    
    return NRF24L01_writeByte(address, temp);
}

static uint8_t NRF24L01_readBits(uint8_t address, uint8_t start, uint8_t len)
{
    uint8_t mask = (1 << len) - 1;
    uint8_t data = NRF24L01_readByte(address);
    
    return (data >> start) & mask;
}

static uint8_t NRF24L01_writeBit(uint8_t address, size_t bit, bool value)
{
    uint8_t data = NRF24L01_readByte(address);
    
    if(value)
        data |= (1 << bit);
    else
        data &= ~(1 << bit);
    
    return NRF24L01_writeByte(address, data);
}

static bool NRF24L01_readBit(uint8_t address, size_t bit)
{
    uint8_t data = NRF24L01_readByte(address);
    
    return (data & (1 << bit))?true:false;
}


static void NRF24L01_readRXPayload(uint8_t* data, uint8_t len)
{
    NRF24L01_command(NRF24L01_CMD_R_RX_PAYLOAD, data, len);
}

static void NRF24L01_writeTXPayload(uint8_t* data, uint8_t len)
{
    uint8_t temp[NRF24L01_MAX_PAYLOAD];
    
    memcpy(temp, data, len);
    
    NRF24L01_command(NRF24L01_CMD_W_TX_PAYLOAD, temp, len);
}

static void NRF24L01_flushTX(void)
{
    NRF24L01_command(NRF24L01_CMD_FLUSH_TX, NULL, 0);
}

static void NRF24L01_flushRX(void)
{
    NRF24L01_command(NRF24L01_CMD_FLUSH_RX, NULL, 0);
}

static void NRF24L01_setReuseTXEnabled(void)
{
    NRF24L01_command(NRF24L01_CMD_REUSE_TX_PL, NULL, 0);
}

static uint8_t NRF24L01_getRXPayloadWidth(void)
{
    uint8_t data;
    
    NRF24L01_command(NRF24L01_CMD_R_RX_PL_WID, &data, 1);
    
    return data;
}

static void NRF24L01_writeAcknowledgePayload(uint8_t pipe,
    uint8_t* data, uint8_t len)
{
    NRF24L01_command(NRF24L01_CMD_W_ACK_PAYLOAD(pipe), data, len);
}

static void NRF24L01_lockUnlock(void)
{
    uint8_t data = NRF24L01_CMD_ACTIVATE_DATA;
    NRF24L01_command(NRF24L01_CMD_ACTIVATE, &data, 1);
}

static uint8_t NRF24L01_getStatus(void)
{
    return NRF24L01_command(NRF24L01_CMD_NOP, NULL, 0);
}


//Registers
//Config
static bool NRF24L01_getInterruptRXDREnabled(void)
{
    return NRF24L01_readBit(NRF24L01_RA_CONFIG, NRF24L01_BIT_MASK_RX_DR);
}

static void NRF24L01_setInterruptRXDREnabled(bool enabled)
{
    NRF24L01_writeBit(NRF24L01_RA_CONFIG, NRF24L01_BIT_MASK_RX_DR, enabled);
}

static bool NRF24L01_getInterruptTXDSEnabled(void)
{
    return NRF24L01_readBit(NRF24L01_RA_CONFIG, NRF24L01_BIT_MASK_TX_DS);
}

static void NRF24L01_setInterruptTXDSEnabled(bool enabled)
{
    NRF24L01_writeBit(NRF24L01_RA_CONFIG, NRF24L01_BIT_MASK_TX_DS, enabled);
}

static bool NRF24L01_getInterruptMAXRTEnabled(void)
{
    return NRF24L01_readBit(NRF24L01_RA_CONFIG, NRF24L01_BIT_MASK_MAX_RT);
}

static void NRF24L01_setInterruptMAXRTEnabled(bool enabled)
{
    NRF24L01_writeBit(NRF24L01_RA_CONFIG, NRF24L01_BIT_MASK_MAX_RT, enabled);
}

static bool NRF24L01_getCRCEnabled(void)
{
    return NRF24L01_readBit(NRF24L01_RA_CONFIG, NRF24L01_BIT_EN_CRC);
}

static void NRF24L01_setCRCEnabled(bool enabled)
{
    NRF24L01_writeBit(NRF24L01_RA_CONFIG, NRF24L01_BIT_EN_CRC, enabled);
}

static bool NRF24L01_getCRCBytes(void)
{
    return NRF24L01_readBit(NRF24L01_RA_CONFIG, NRF24L01_BIT_CRCO);
}

static void NRF24L01_setCRCBytes(bool twoBytes)
{
    NRF24L01_writeBit(NRF24L01_RA_CONFIG, NRF24L01_BIT_CRCO, twoBytes);
}

static bool NRF24L01_getPowerUpEnabled(void)
{
    return NRF24L01_readBit(NRF24L01_RA_CONFIG, NRF24L01_BIT_PWR_UP);
}

static void NRF24L01_setPowerUpEnabled(bool enabled)
{
    NRF24L01_writeBit(NRF24L01_RA_CONFIG, NRF24L01_BIT_PWR_UP, enabled);
}

static bool NRF24L01_getPrimRXEnabled(void)
{
    return NRF24L01_readBit(NRF24L01_RA_CONFIG, NRF24L01_BIT_PRIM_RX);
}

static void NRF24L01_setPrimRXEnabled(bool enabled)
{
    NRF24L01_writeBit(NRF24L01_RA_CONFIG, NRF24L01_BIT_PRIM_RX, enabled);
}

//Enable auto acknowledge
static bool NRF24L01_getAutoAcknowledgeEnabled(uint8_t pipe)
{
    return NRF24L01_readBit(NRF24L01_RA_EN_AA, NRF24L01_BIT_ENAA_P(pipe));
}

static void NRF24L01_setAutoAcknowledgeEnabled(uint8_t pipe, bool enabled)
{
    if(pipe == NRF24L01_PIPES_N)
        NRF24L01_writeBits(NRF24L01_RA_EN_AA, NRF24L01_BIT_ENAA_P0,
            NRF24L01_PIPES_N, (enabled)?0xFF:0x00);
    else
        NRF24L01_writeBit(NRF24L01_RA_EN_AA, NRF24L01_BIT_ENAA_P(pipe),
            enabled);
}

//Enable RX pipe
static bool NRF24L01_getPipeEnabled(uint8_t pipe)
{
    return NRF24L01_readBit(NRF24L01_RA_EN_RXADDR, NRF24L01_BIT_ERX_P(pipe));
}

static void NRF24L01_setPipeEnabled(uint8_t pipe, bool enabled)
{
    if(pipe == NRF24L01_PIPES_N)
        NRF24L01_writeBits(NRF24L01_RA_EN_RXADDR, NRF24L01_BIT_ERX_P0,
            NRF24L01_PIPES_N, (enabled)?0xFF:0x00);
    else
        NRF24L01_writeBit(NRF24L01_RA_EN_RXADDR, NRF24L01_BIT_ERX_P(pipe),
            enabled);
}

//Address width
static uint8_t NRF24L01_getAddressWidth(void)
{
    uint8_t data = NRF24L01_readBits(NRF24L01_RA_SETUP_AW, NRF24L01_BIT_AW,
        NRF24L01_LEN_AW);
    
    if(data == 0x01)
        return 3;
    else if(data == 0x02)
        return 4;
    else
        return 5;
}

static void NRF24L01_setAddressWidth(uint8_t width)
{
    uint8_t data;
    
    if(width == 3)
        data = 0x01;
    else if(width == 4)
        data = 0x02;
    else
        data = 0x03;
    
    NRF24L01_writeBits(NRF24L01_RA_SETUP_AW, NRF24L01_BIT_AW, NRF24L01_LEN_AW,
        data);
}

//Retries
static uint16_t NRf24L01_getAutoRetransmitDelay(void)
{
    uint16_t data = NRF24L01_readBits(NRF24L01_RA_SETUP_RETR, NRF24L01_BIT_ARD,
        NRF24L01_LEN_ARD);
    
    return 250 * (data + 1);
}

static void NRF24L01_setAutoRetransmitDelay(uint16_t delay)
{
    uint8_t data = delay / 250 - 1;
    
    NRF24L01_writeBits(NRF24L01_RA_SETUP_RETR, NRF24L01_BIT_ARD,
        NRF24L01_LEN_ARD, data);
}

static uint8_t NRf24L01_getAutoRetransmitCount(void)
{
    return NRF24L01_readBits(NRF24L01_RA_SETUP_RETR, NRF24L01_BIT_ARC,
        NRF24L01_LEN_ARC);
}

static void NRF24L01_setAutoRetransmitCount(uint8_t retries)
{
    NRF24L01_writeBits(NRF24L01_RA_SETUP_RETR, NRF24L01_BIT_ARC,
        NRF24L01_LEN_ARC, retries);
}

//RF channel
static uint8_t NRF24L01_getRFChannel(void)
{
    return NRF24L01_readBits(NRF24L01_RA_RF_CH, NRF24L01_BIT_RF_CH,
        NRF24L01_LEN_RF_CH);
}

static void NRF24L01_setRFChannel(uint8_t channel)
{
    NRF24L01_writeBits(NRF24L01_RA_RF_CH, NRF24L01_BIT_RF_CH,
        NRF24L01_LEN_RF_CH, channel);
}

//RF setup
static bool NRF24L01_getContinuousCarrierEnabled(void)
{
    return NRF24L01_readBit(NRF24L01_RA_RF_SETUP, NRF24L01_BIT_CONT_WAVE);
}

static void NRF24L01_setContinuousCarrierEnabled(bool enabled)
{
    NRF24L01_writeBit(NRF24L01_RA_RF_SETUP, NRF24L01_BIT_CONT_WAVE, enabled);
}

static bool NRF24L01_getDataRate250kbpsEnabled(void)
{
    return NRF24L01_readBit(NRF24L01_RA_RF_SETUP, NRF24L01_BIT_RF_DR_LOW);
}

static void NRF24L01_setDataRate250kbpsEnabled(bool enabled)
{
    NRF24L01_writeBit(NRF24L01_RA_RF_SETUP, NRF24L01_BIT_RF_DR_LOW, enabled);
}

static bool NRF24L01_getPLLLockEnabled(void)
{
    return NRF24L01_readBit(NRF24L01_RA_RF_SETUP, NRF24L01_BIT_PLL_LOCK);
}

static void NRF24L01_setPLLLockEnabled(bool enabled)
{
    NRF24L01_writeBit(NRF24L01_RA_RF_SETUP, NRF24L01_BIT_PLL_LOCK, enabled);
}

static bool NRF24L01_getHighDataRateEnabled(void)
{
    return NRF24L01_readBit(NRF24L01_RA_RF_SETUP, NRF24L01_BIT_RF_DR_HIGH);
}

static void NRF24L01_setHighDataRateEnabled(bool enabled)
{
    NRF24L01_writeBit(NRF24L01_RA_RF_SETUP, NRF24L01_BIT_RF_DR_HIGH, enabled);
}

static uint8_t NRF24L01_getOutputPower(void)
{
    return NRF24L01_readBits(NRF24L01_RA_RF_SETUP, NRF24L01_BIT_RF_PWR,
        NRF24L01_LEN_RF_PWR);
}

static void NRF24L01_setOutputPower(uint8_t power)
{
    NRF24L01_writeBits(NRF24L01_RA_RF_SETUP, NRF24L01_BIT_RF_PWR,
        NRF24L01_LEN_RF_PWR, power);
}

//Status
static uint8_t NRF24L01_getInterrupts(void)
{
    return NRF24L01_getStatus()
        & ((1 << NRF24L01_BIT_RX_DR) | (1 << NRF24L01_BIT_TX_DS)
            | (1 << NRF24L01_BIT_MAX_RT));
}

static uint8_t NRF24L01_getClearInterrupts(void)
{
    uint8_t data = NRF24L01_getInterrupts();
    
    NRF24L01_writeByte(NRF24L01_RA_STATUS, data);
    
    return data;
}

static void NRF24L01_clearRXDRInterrupt(void)
{
    NRF24L01_writeBit(NRF24L01_RA_STATUS, NRF24L01_BIT_RX_DR, 1);
}

static void NRF24L01_clearTXDSInterrupt(void)
{
    NRF24L01_writeBit(NRF24L01_RA_STATUS, NRF24L01_BIT_TX_DS, 1);
}

static void NRF24L01_clearMAXRTInterrupt(void)
{
    NRF24L01_writeBit(NRF24L01_RA_STATUS, NRF24L01_BIT_MAX_RT, 1);
}

static uint8_t NRF24L01_getRXPipe(void)
{
    return NRF24L01_readBits(NRF24L01_RA_STATUS, NRF24L01_BIT_RX_P_NO,
        NRF24L01_LEN_RX_P_NO);
}

//Observe TX
static uint8_t NRF24L01_getLostPackets(void)
{
    return NRF24L01_readBits(NRF24L01_RA_OBSERVE_TX, NRF24L01_BIT_PLOS_CNT,
        NRF24L01_LEN_PLOS_CNT);
}

static uint8_t NRF24L01_getRetransmittedPackets(void)
{
    return NRF24L01_readBits(NRF24L01_RA_OBSERVE_TX, NRF24L01_BIT_ARC_CNT,
        NRF24L01_LEN_ARC_CNT);
}

//Received power detector
static bool NRF24L01_getCarrierDetected(void)
{
    return NRF24L01_readBit(NRF24L01_RA_RPD, NRF24L01_BIT_RPD);
}

//RX addresses
static void NRF24L01_getRXAddress(uint8_t pipe, uint8_t* address)
{
    NRF24L01_read(NRF24L01_RA_RX_ADDR_P(pipe), address,
        NRF24L01_RA_RX_ADDR_P_LEN(pipe));
}

static void NRF24L01_setRXAddress(uint8_t pipe, uint8_t* address)
{
   NRF24L01_write(NRF24L01_RA_RX_ADDR_P(pipe), address,
    NRF24L01_RA_RX_ADDR_P_LEN(pipe));
}

//TX address
static void NRF24L01_getTXAddress(uint8_t* address)
{
    NRF24L01_read(NRF24L01_RA_TX_ADDR, address, NRF24L01_RA_TX_ADDR_LEN);
}

static void NRF24L01_setTXAddress(uint8_t* address)
{
    NRF24L01_write(NRF24L01_RA_TX_ADDR, address, NRF24L01_RA_TX_ADDR_LEN);
}

//RX payload widths
static uint8_t NRF24L01_getPayloadWidth(uint8_t pipe)
{
    return NRF24L01_readBits(NRF24L01_RA_RX_PW_P(pipe), NRF24L01_BIT_RX_PW_P,
        NRF24L01_LEN_RX_PW_P);
}

static void NRF24L01_setPayloadWidth(uint8_t pipe, uint8_t width)
{
    NRF24L01_writeBits(NRF24L01_RA_RX_PW_P(pipe), NRF24L01_BIT_RX_PW_P,
        NRF24L01_LEN_RX_PW_P, width);
}

//FIFO status
static bool NRF24L01_getReuseTXEnabled(void)
{
    return NRF24L01_readBit(NRF24L01_RA_FIFO_STATUS, NRF24L01_BIT_TX_REUSE);
}

static bool NRF24L01_getTXFIFOFull(void)
{
    return NRF24L01_readBit(NRF24L01_RA_FIFO_STATUS, NRF24L01_BIT_FIFO_TX_FULL);
}

static bool NRF24L01_getTXFIFOEmpty(void)
{
    return NRF24L01_readBit(NRF24L01_RA_FIFO_STATUS, NRF24L01_BIT_TX_EMPTY);
}

static bool NRF24L01_getRXFIFOFull(void)
{
    return NRF24L01_readBit(NRF24L01_RA_FIFO_STATUS, NRF24L01_BIT_RX_FULL);
}

static bool NRF24L01_getRXFIFOEmpty(void)
{
    return NRF24L01_readBit(NRF24L01_RA_FIFO_STATUS, NRF24L01_BIT_RX_EMPTY);
}

//Dynamic payload
static bool NRF24L01_getDynamicPayloadPipeEnabled(uint8_t pipe)
{
    return NRF24L01_readBit(NRF24L01_RA_DYNPD, NRF24L01_BIT_DPL_P(pipe));
}

static void NRF24L01_setDynamicPayloadPipeEnabled(uint8_t pipe, bool enabled)
{
    if(pipe == NRF24L01_PIPES_N)
        NRF24L01_writeBits(NRF24L01_RA_DYNPD, NRF24L01_BIT_DPL_P0,
            NRF24L01_PIPES_N, (enabled)?0xFF:0x00);
    else
        NRF24L01_writeBit(NRF24L01_RA_DYNPD, NRF24L01_BIT_DPL_P(pipe), enabled);
}

//Features
static bool NRF24L01_getDynamicPayloadEnabled(void)
{
    return NRF24L01_readBit(NRF24L01_RA_FEATURE, NRF24L01_BIT_EN_DPL);
}

static void NRF24L01_setDynamicPayloadEnabled(bool enabled)
{
    NRF24L01_writeBit(NRF24L01_RA_FEATURE, NRF24L01_BIT_EN_DPL, enabled);
}

static bool NRF24L01_getAcknowledgePayloadEnabled(void)
{
    return NRF24L01_readBit(NRF24L01_RA_FEATURE, NRF24L01_BIT_EN_ACK_PAY);
}

static void NRF24L01_setAcknowledgePayloadEnabled(bool enabled)
{
    NRF24L01_writeBit(NRF24L01_RA_FEATURE, NRF24L01_BIT_EN_ACK_PAY, enabled);
}

static bool NRF24L01_getDynamicAcknowledgeEnabled(void)
{
    return NRF24L01_readBit(NRF24L01_RA_FEATURE, NRF24L01_BIT_EN_DYN_ACK);
}

void NRF24L01_setDynamicAcknowledgeEnabled(bool enabled)
{
    NRF24L01_writeBit(NRF24L01_RA_FEATURE, NRF24L01_BIT_EN_DYN_ACK, enabled);
}



void NRF24L01_init(uint8_t* csnDDR, uint8_t* csnPORT, uint8_t csnPin,
    uint8_t* ceDDR, uint8_t* cePORT, uint8_t cePin)
{
    NRF24L01_csnPORT = csnPORT;
    NRF24L01_csnPin = csnPin;
    NRF24L01_cePORT = cePORT;
    NRF24L01_cePin = cePin;
    
    *csnDDR |= (1 << NRF24L01_csnPin);
    *ceDDR |= (1 << NRF24L01_cePin);
    NRF24L01_setCSN(true);
    NRF24L01_setCE(false);
    
    
    //10Mhz, Msb first, Mode 0
    //SPI_init(10000000, 0, 0, 0);
    
    
    
    _delay_ms(100);
    
    //Close all pipes
    NRF24L01_setPipeEnabled(NRF24L01_PIPES_N, false);
    NRF24L01_setAutoAcknowledgeEnabled(NRF24L01_PIPES_N, false);
    
    
    //Open pipe 0
    NRF24L01_setPipeEnabled(0, true);
    NRF24L01_setPayloadWidth(0, NRF24L01_MAX_PAYLOAD);
    NRF24L01_setAutoAcknowledgeEnabled(0, true);
    NRF24L01_setDynamicPayloadPipeEnabled(0, true);
    //CRC 2 bytes
    NRF24L01_setCRCBytes(true);
    //Auto retransmit
    NRF24L01_setAutoRetransmitDelay(500);
    NRF24L01_setAutoRetransmitCount(15);
    //ACK payload, dynamic payload width
    NRF24L01_setAcknowledgePayloadEnabled(true);
    if(NRF24L01_readByte(NRF24L01_RA_FEATURE) == 0x00
        && NRF24L01_readByte((NRF24L01_RA_DYNPD) == 0x00))
    {
        NRF24L01_lockUnlock();
        NRF24L01_setAcknowledgePayloadEnabled(true);
    }
    NRF24L01_setDynamicPayloadEnabled(true);
    NRF24L01_setDynamicPayloadPipeEnabled(NRF24L01_PIPES_N, true);
    //Enable RX
    NRF24L01_setPrimRXEnabled(true);
    
    //Power up
    NRF24L01_setPowerUpEnabled(true);
    _delay_us(150);
    
    
    
    //Listen
    NRF24L01_setCE(true);
}



bool NRF24L01_sendData(uint8_t* data, uint8_t len)
{
    uint8_t interrupts;
    
    
    
    NRF24L01_setCE(false);
    
    NRF24L01_flushTX();
    NRF24L01_writeTXPayload(data, len);
    
    NRF24L01_setPrimRXEnabled(false);
    NRF24L01_setCE(true);
    _delay_us(10);
    NRF24L01_setCE(false);
    
    
    do
    {
        interrupts = NRF24L01_getInterrupts();
    }
    while(!(interrupts
        & ((1 << NRF24L01_BIT_TX_DS) | (1 << NRF24L01_BIT_MAX_RT))));
    NRF24L01_clearTXDSInterrupt();
    NRF24L01_clearMAXRTInterrupt();
    
    
    if(interrupts & (1 << NRF24L01_BIT_MAX_RT))
        NRF24L01_flushTX();
    
    NRF24L01_setPrimRXEnabled(true);
    NRF24L01_setCE(true);
    
    
    
    return interrupts & (1 << NRF24L01_BIT_MAX_RT);
}

void NRF24L01_setAcknowledgePayload(uint8_t* data, uint8_t len)
{
    NRF24L01_flushTX();
    NRF24L01_writeAcknowledgePayload(0, data, len);
}


bool NRF24L01_dataAvailable(void)
{
    return !NRF24L01_getRXFIFOEmpty();
}

uint8_t NRF24L01_getData(uint8_t* data)
{
    uint8_t len = 0;
    
    if(NRF24L01_getRXPipe() < NRF24L01_PIPES_N)
    {
        len = NRF24L01_getRXPayloadWidth();
        NRF24L01_readRXPayload(data, len);
    }
    
    return len;
}
