#define F_CPU 73728UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


// NRF24L01-mnemonics.h
/**
 *  From the specifications available in the download section of
 *  http://www.nordicsemi.com/kor/Products/2.4GHz-RF/nRF24L01P
 */

// SPI Commands
#define R_REGISTER          0x00 // 000A AAAA
#define W_REGISTER          0x20 // 001A AAAA
#define R_RX_PAYLOAD        0x61 // 0110 0001
#define W_TX_PAYLOAD        0xA0 // 1010 0000
#define FLUSH_TX            0xE1 // 1110 0001
#define FLUSH_RX            0xE2 // 1110 0010
#define REUSE_TX_PL         0xE3 // 1110 0011
#define R_RX_PL_WID         0x60 // 0110 0000
#define W_ACK_PAYLOAD       0xA8 // 1010 1PPP
#define W_TX_PAYLOAD_NOACK  0xB0 // 1011 0000
#define NOP                 0xFF // 1111 1111

// Register Map
#define CONFIG      0x00
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0

#define EN_AA       0x01
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0

#define EN_RXADDR   0x02
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0

#define SETUP_AW    0x03
#define AW          0

#define SETUP_RETR  0x04
#define ARD         4
#define ARC         0

#define RF_CH       0x05

#define RF_SETUP    0x06
#define CONT_WAVE   7
#define RF_DR_LOW   5
#define PLL_LOCK    4
#define RF_DR_HIGH  3
#define RF_PWR      1

#define STATUS              0x07
#define RX_DR               6
#define TX_DS               5
#define MAX_RT              4
#define RX_P_NO_MASK        0x0E
#define STATUS_TX_FULL      0

#define OBSERVE_TX  0x08
#define PLOS_CNT    4
#define ARC_CNT     0

#define RPD         0x09

#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F

#define TX_ADDR     0x10

#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16

#define FIFO_STATUS         0x17
#define TX_REUSE            6
#define FIFO_TX_FULL        5
#define TX_EMPTY            4
#define RX_FULL             1
#define RX_EMPTY            0

#define DYNPD       0x1C
#define DPL_P5      5
#define DPL_P4      4
#define DPL_P3      3
#define DPL_P2      2
#define DPL_P1      1
#define DPL_P0      0

#define FEATURE     0x1D
#define EN_DPL      2
#define EN_ACK_PAY  1
#define EN_DYN_ACK  0


typedef struct
{
    int pipe_number;
    uint8_t data[32];
    uint8_t length;
} nRF24L01Message;

typedef struct
{
    volatile uint8_t *port;
    uint8_t pin;
} gpio_pin;

typedef struct
{
    gpio_pin ss; // slave select
    gpio_pin ce; // chip enabled
    gpio_pin sck; // serial clock
    gpio_pin mosi; // master out slave in
    gpio_pin miso; // master in slave out
    uint8_t status;
} nRF24L01;


nRF24L01 *nRF24L01_init(void);
void nRF24L01_begin(nRF24L01 *rf);
uint8_t nRF24L01_send_command(nRF24L01 *rf, uint8_t command, void *data, size_t length);
uint8_t nRF24L01_write_register(nRF24L01 *rf, uint8_t reg_address, void *data, size_t length);
uint8_t nRF24L01_read_register(nRF24L01 *rf, uint8_t regAddress, void *data, size_t length);
uint8_t nRF24L01_no_op(nRF24L01 *rf);
uint8_t nRF24L01_update_status(nRF24L01 *rf);
uint8_t nRF24L01_get_status(nRF24L01 *rf);
void nRF24L01_listen(nRF24L01 *rf, int pipe, uint8_t *address);
bool nRF24L01_data_received(nRF24L01 *rf);
bool nRF24L01_read_received_data(nRF24L01 *rf, nRF24L01Message *message);
int nRF24L01_pipe_number_received(nRF24L01 *rf);
void nRF24L01_transmit(nRF24L01 *rf, void *address, nRF24L01Message *msg);
int nRF24L01_transmit_success(nRF24L01 *rf);
void nRF24L01_flush_transmit_message(nRF24L01 *rf);
void nRF24L01_retry_transmit(nRF24L01 *rf);
void nRF24L01_clear_interrupts(nRF24L01 *rf);
void nRF24L01_clear_transmit_interrupts(nRF24L01 *rf);
void nRF24L01_clear_receive_interrupt(nRF24L01 *rf);

// SN - 21.12.2017
void nRF24L01_setToStandbyMode(nRF24L01 *rf);
// SN - 21.12.2017

static void copy_address(uint8_t *source, uint8_t *destination);
inline static void set_as_output(gpio_pin pin);
inline static void set_high(gpio_pin pin);
inline static void set_low(gpio_pin pin);
static void spi_init(nRF24L01 *rf);
static uint8_t spi_transfer(uint8_t data);


nRF24L01 *nRF24L01_init(void) 
{
    nRF24L01 *rf = malloc(sizeof(nRF24L01));
    memset(rf, 0, sizeof(nRF24L01));
    return rf;
}


void nRF24L01_begin(nRF24L01 *rf) 
{
    set_as_output(rf->ss);
    set_as_output(rf->ce);

    set_high(rf->ss);
    set_low(rf->ce);

    spi_init(rf);

    nRF24L01_send_command(rf, FLUSH_RX, NULL, 0);
    nRF24L01_send_command(rf, FLUSH_TX, NULL, 0);
    nRF24L01_clear_interrupts(rf);

    uint8_t data;
    data = _BV(EN_CRC) | _BV(CRCO) | _BV(PWR_UP) | _BV(PRIM_RX);
    nRF24L01_write_register(rf, CONFIG, &data, 1);

    // enable Auto Acknowlegde on all pipes
    data = _BV(ENAA_P0) | _BV(ENAA_P1) | _BV(ENAA_P2) | _BV(ENAA_P3) | _BV(ENAA_P4) | _BV(ENAA_P5);
    nRF24L01_write_register(rf, EN_AA, &data, 1);

    // enable Dynamic Payload on al pipes
    data = _BV(DPL_P0) | _BV(DPL_P1) | _BV(DPL_P2) | _BV(DPL_P3) | _BV(DPL_P4) | _BV(DPL_P5);
    nRF24L01_write_register(rf, DYNPD, &data, 1);

    // enable Dynamic Payload (global)
    data = _BV(EN_DPL);
    nRF24L01_write_register(rf, FEATURE, &data, 1);

    // disable all rx addresses
    data = 0;
    nRF24L01_write_register(rf, EN_RXADDR, &data, 1);
}


uint8_t nRF24L01_send_command(nRF24L01 *rf, uint8_t command, void *data, size_t length) 
{
    set_low(rf->ss);

    rf->status = spi_transfer(command);
    for (unsigned int i = 0; i < length; i++)
        ((uint8_t*)data)[i] = spi_transfer(((uint8_t*)data)[i]);

    set_high(rf->ss);

    return rf->status;
}


uint8_t nRF24L01_write_register(nRF24L01 *rf, uint8_t reg_address, void *data, size_t length) 
{
    return nRF24L01_send_command(rf, W_REGISTER | reg_address, data, length);
}


uint8_t nRF24L01_read_register(nRF24L01 *rf, uint8_t reg_address, void *data, size_t length)
{
    return nRF24L01_send_command(rf, R_REGISTER | reg_address, data, length);
}


uint8_t nRF24L01_no_op(nRF24L01 *rf) 
{
    return nRF24L01_send_command(rf, NOP, NULL, 0);
}


uint8_t nRF24L01_update_status(nRF24L01 *rf) 
{
    return nRF24L01_no_op(rf);
}


uint8_t nRF24L01_get_status(nRF24L01 *rf) 
{
    return rf->status;
}


bool nRF24L01_data_received(nRF24L01 *rf) 
{
    set_low(rf->ce);
    nRF24L01_update_status(rf);
    return nRF24L01_pipe_number_received(rf) >= 0;
}


void nRF24L01_listen(nRF24L01 *rf, int pipe, uint8_t *address) 
{
    uint8_t addr[5];
    copy_address(address, addr);

    nRF24L01_write_register(rf, RX_ADDR_P0 + pipe, addr, 5);

    uint8_t current_pipes;
    nRF24L01_read_register(rf, EN_RXADDR, &current_pipes, 1);
    current_pipes |= _BV(pipe);
    nRF24L01_write_register(rf, EN_RXADDR, &current_pipes, 1);

    set_high(rf->ce);
}


bool nRF24L01_read_received_data(nRF24L01 *rf, nRF24L01Message *message) 
{
    message->pipe_number = nRF24L01_pipe_number_received(rf);
    nRF24L01_clear_receive_interrupt(rf);
    if (message->pipe_number < 0) 
    {
        message->length = 0;
        return false;
    }

    nRF24L01_read_register(rf, R_RX_PL_WID, &message->length, 1);

    if (message->length > 0) 
    {
        nRF24L01_send_command(rf, R_RX_PAYLOAD, &message->data,
            message->length);
    }

    return true;
}


int nRF24L01_pipe_number_received(nRF24L01 *rf) 
{
    int pipe_number = (rf->status & RX_P_NO_MASK) >> 1;
    return pipe_number <= 5 ? pipe_number : -1;
}


void nRF24L01_transmit(nRF24L01 *rf, void *address, nRF24L01Message *msg) 
{
    nRF24L01_clear_transmit_interrupts(rf);
    uint8_t addr[5];
    copy_address((uint8_t *)address, addr);
    nRF24L01_write_register(rf, TX_ADDR, addr, 5);
    copy_address((uint8_t *)address, addr);
    nRF24L01_write_register(rf, RX_ADDR_P0, addr, 5);
    nRF24L01_send_command(rf, W_TX_PAYLOAD, &msg->data, msg->length);
    uint8_t config;
    nRF24L01_read_register(rf, CONFIG, &config, 1);
    config &= ~_BV(PRIM_RX);
    nRF24L01_write_register(rf, CONFIG, &config, 1);
    set_high(rf->ce);
}


int nRF24L01_transmit_success(nRF24L01 *rf) 
{
    set_low(rf->ce);
    nRF24L01_update_status(rf);
    int success;
    if (rf->status & _BV(TX_DS)) 
        success = 0;
    else if (rf->status & _BV(MAX_RT)) 
        success = -1;
    else 
        success = -2;
    nRF24L01_clear_transmit_interrupts(rf);
    uint8_t config;
    nRF24L01_read_register(rf, CONFIG, &config, 1);
    config |= _BV(PRIM_RX);
    nRF24L01_write_register(rf, CONFIG, &config, 1);
    return success;
}


void nRF24L01_flush_transmit_message(nRF24L01 *rf) 
{
    nRF24L01_send_command(rf, FLUSH_TX, NULL, 0);
}


void nRF24L01_retry_transmit(nRF24L01 *rf) 
{
    // XXX not sure it works this way, never tested
    uint8_t config;
    nRF24L01_read_register(rf, CONFIG, &config, 1);
    config &= ~_BV(PRIM_RX);
    nRF24L01_write_register(rf, CONFIG, &config, 1);
    set_high(rf->ce);
}


void nRF24L01_clear_interrupts(nRF24L01 *rf) 
{
    uint8_t data = _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT);
    nRF24L01_write_register(rf, STATUS, &data, 1);
}



void nRF24L01_clear_transmit_interrupts(nRF24L01 *rf) 
{
    uint8_t data = _BV(TX_DS) | _BV(MAX_RT);
    nRF24L01_write_register(rf, STATUS, &data, 1);
}


void nRF24L01_clear_receive_interrupt(nRF24L01 *rf) 
{
    uint8_t data = _BV(RX_DR) | rf->status;
    nRF24L01_write_register(rf, STATUS, &data, 1);
}


static void copy_address(uint8_t *source, uint8_t *destination) 
{
    for (int i = 0; i < 5; i++)
        destination[i] = source[i];
}


inline static void set_as_output(gpio_pin pin) 
{
    volatile uint8_t *ddr = pin.port - 1;
    *ddr |= _BV(pin.pin);
}


inline static void set_as_input(gpio_pin pin) 
{
    volatile uint8_t *ddr = pin.port - 1;
    *ddr &= ~_BV(pin.pin);
}


inline static void set_high(gpio_pin pin) 
{
    *pin.port |= _BV(pin.pin);
}


inline static void set_low(gpio_pin pin) 
{
    *pin.port &= ~_BV(pin.pin);
}


static void spi_init(nRF24L01 *rf) 
{
    // set as master
    SPCR |= _BV(MSTR);
    // enable SPI
    SPCR |= _BV(SPE);
    // MISO pin automatically overrides to input
    set_as_output(rf->sck);
    set_as_output(rf->mosi);
    set_as_input(rf->miso);
    // SPI mode 0: Clock Polarity CPOL = 0, Clock Phase CPHA = 0
    SPCR &= ~_BV(CPOL);
    SPCR &= ~_BV(CPHA);
    // Clock 2X speed
    SPCR &= ~_BV(SPR0);
    SPCR &= ~_BV(SPR1);
    SPSR |= _BV(SPI2X);
    // most significant first (MSB)
    SPCR &= ~_BV(DORD);
}


static uint8_t spi_transfer(uint8_t data) 
{
    SPDR = data;
    while (!(SPSR & _BV(SPIF)))
        ;
    return SPDR;
}


// (SN) - 21.12.2017
void nRF24L01_setToStandbyMode(nRF24L01 *rf)
{
    set_low(rf->ce);
    nRF24L01_clear_interrupts(rf);
}


// --------------------------------------------------------
// (SN) - 22.12.2017 - AddLib
// DONE für OPEN-M-128
#define MAXSTR 30

nRF24L01 *rf = NULL;
uint8_t to_address[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };
char nRF24L01_received_string[MAXSTR+1]="";
volatile bool modeListen = true;
volatile bool nRF24L01DataReady = false;

void process_message(char *message);

// Aendern fuer andere Hardware !! ------------------------
nRF24L01 *setup_rf(void)
{
    nRF24L01 *rf = nRF24L01_init();
    rf->ss.port = &PORTB;
    rf->ss.pin = PB2;
    rf->ce.port = &PORTB;
    rf->ce.pin = PB1;
    rf->sck.port = &PORTB;
    rf->sck.pin = PB5;
    rf->mosi.port = &PORTB;
    rf->mosi.pin = PB3;
    rf->miso.port = &PORTB;
    rf->miso.pin = PB4;
    // interrupt on falling edge of INT0 (PD2)
    EICRA |= 1 << ISC01;
    EIMSK |= 1 << INT0;

    nRF24L01_begin(rf);
    return rf;
}
// --------------------------------------------------------


bool nRF24L01_init_system(void)
{
    rf = setup_rf();
    nRF24L01_listen(rf,0,to_address);

    return true;
}


// Interruptnummer aendern fuer andere Hardware -----------
// nRF24L01 interrupt
ISR(INT0_vect)
{
    if(modeListen == false)
    {
        int success = nRF24L01_transmit_success(rf);
        if (success != 0)
        {
            nRF24L01_flush_transmit_message(rf);
        }
                    
        modeListen = true;
        nRF24L01_listen(rf,0,to_address);
    }
    else
    {
        if (nRF24L01_data_received(rf))
        {
            nRF24L01Message msg;
            nRF24L01_read_received_data(rf, &msg);
            process_message((char *)msg.data);
            nRF24L01DataReady = true;
        }

        nRF24L01_listen(rf, 0, to_address);
    }
}


void nRF24L01_send_string(char * s)
{
    modeListen = false;
    nRF24L01Message msg;
    strncpy((char*)msg.data,s,MAXSTR);
    msg.length = strlen((char *)msg.data) + 1;
    nRF24L01_setToStandbyMode(rf);
    nRF24L01_transmit(rf, to_address, &msg);
}


void process_message(char *message)
{
    strncpy(nRF24L01_received_string,message,MAXSTR);
}


bool nRF24L01_data_ready(void)
{
    return nRF24L01DataReady;
}


bool nRF24L01_read_string(char *s)
{
    if(!nRF24L01DataReady)
      return false;
    strncpy(s,nRF24L01_received_string,MAXSTR);
    nRF24L01DataReady = false;
    return true;
}

void nRF24L01_set_address(char address)
{
    int i;

    for(i=0;i<5;i++)
    {
        to_address[i]=address;
    }
    nRF24L01_setToStandbyMode(rf);
    nRF24L01_listen(rf,0,to_address);
}

char nRF24L01_get_address(void)
{
    return to_address[0];
}
