#define F_CPU 7372800UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "nrf24l01lib.h"
#include "Uarts.h"

#define MAXSTR 30

void init(void);
void initPorts(void);
void delay(int ms);

int getADC(int kanal);
bool processRequestSlave(char *request, char *response);
bool doConfig(char *str, char *retStr);

char *version = "NRF2401-RXTX-LibTestTemplate (OPEN-M-128) V1.03 (28.12.2017) [SN]";
char rec0str[MAXSTR] = "";
volatile bool rx0flag = false;

// Konfiguration fuer das System
bool _master = true;    // default: master
bool _echo   = true;    // default: echo on


int main(void)
{
    char string[MAXSTR+1]="";
    char returnString[80]="";

    init();
    strUart0(version);
    strUart0("\r");
    nRF24L01_init_system();

    while (true)
    {
        if(nRF24L01_data_ready())   // sind Daten da?
        {
            // einlesen und Flag zuruecksetzen
            nRF24L01_read_string(string);
            // empfangene Nachricht ggf ueber die 
            // serielle Schnittstelle senden
            if(_echo || _master)    // Master sendet immer weiter
            {
                strUart0(string);
            }

            if(!_master)    // Slave
            {
                // Antwortstring zusammenstellen
                if(processRequestSlave(string,returnString))
                {
                    // Zuruecksenden
                    nRF24L01_send_string(returnString);
                    // und ggf ueber die serielle Schnittstelle ausgeben.
                    if(_echo)
                    {
                        strUart0(returnString);
                    }
                }
                else // bei Fehler
                {
                    sprintf(returnString,"ERROR:%s\r",string);
                    nRF24L01_send_string(returnString);
                    // und ggf ueber die serielle Schnittstelle ausgeben.
                    if(_echo)
                    {
                        strUart0(returnString);
                    }
                }
            }
        }

        if(rx0flag)                         // serieller String empfangen?
        {
            rx0flag = false;                // Flag zuruecksetzen
            if(rec0str[0]=='#')             // Konfigurations-Befehl?
            {
                if(doConfig(&rec0str[1],returnString))
                {
                    strcat(returnString,"\r");
                    strUart0(returnString);
                }
                else
                {
                    strUart0("Falscher Konfigurationsstring\r");
                }
            }
            else
            {
                strcat(rec0str,"\r");
                nRF24L01_send_string(rec0str);  // String senden
            }
        }
    }

    return 0;
}


void init(void)
{
    initPorts();
    initUarts();

// Ergaenzungen fuer den Slave Betrieb
    ADCSRA= 0x87;           // AD-Wandler
    ADMUX = 0x40;           // VSS-Referenz
    
    UCSR0B |= (1<<RXCIE0);  // Interrupt enable
    sei();
}


void initPorts(void)
{
    DDRA = 0xff;
    PORTA = 0x00;
}


// Serieller Interrupt (RX)
// MAXSTR Zeichen und behandeln bei '\n' oder '\r'
ISR(USART0_RX_vect)
{
    static int cnt=0;
    static char recStrTemp[MAXSTR]="";
    
    recStrTemp[cnt]=UDR0;
    if(recStrTemp[cnt]=='\r' || recStrTemp[cnt]=='\n')
    {
        recStrTemp[cnt]=0;
        cnt=0;
        strcpy(rec0str,recStrTemp);
        rx0flag=true;
    }
    else
    {
        cnt++;
        if(cnt>=MAXSTR)
        {
            cnt=0;      // Selbstschutz
        }
    }
}


// Beispiel 1: Potentiometerwert einlesen (0..3)
int getADC(int kanal)
{
    ADMUX = 0x40|(kanal&0x07);    // nur Einzelkanaele
    ADCSRA|=0x40;
    while((ADCSRA&0x40)==0x40)
        ;
    return ADC;
}


bool processRequestSlave(char *request, char *response)
{
    char *token=strtok(request," ");
    int kanal=0;
    int wert=0;
    int anz=0;
    bool ret = false;

    if(token!=NULL)
    {
        if(!strcmp(token,"GET"))
        {
            token = strtok(NULL," ");
            anz = sscanf(token,"%d",&kanal);
            if(anz==1 && kanal>=0 && kanal<=3)
            {
                wert = getADC(kanal);
                sprintf(response,"%d#%d\r",kanal,wert);
                ret = true;
            }
        }
    }

    return ret;
}
// Ende Beispiel 1


// Konfigurationsstring parsen
bool doConfig(char *str, char *retStr)
{
    bool ret = false;
    char *token = strtok(str," ");
    int addr=0;

    if(!strcmp(token,"SET"))
    {
        token = strtok(NULL, " ");
        if(!strcmp(token,"ADR"))
        {
            token = strtok(NULL, " ");
            sscanf(token,"%x",&addr);
            nRF24L01_set_address(addr);
            sprintf(retStr,"%02x",addr);
            ret=true;
        }
        else if(!strcmp(token,"ECHO"))
        {
            token = strtok(NULL, " ");
            if(!strcmp(token,"ON"))
            {
                _echo = true;
                strcpy(retStr,"ON");
                ret=true;
            }
            else if(!strcmp(token,"OFF"))
            {
                _echo = false;
                strcpy(retStr,"OFF");
                ret=true;
            }
        }
        else if(!strcmp(token,"MASTER"))
        {
            _master = true;
            strcpy(retStr,"MASTER");
            ret=true;
        }
        else if(!strcmp(token,"SLAVE"))
        {
            _master = false;
            strcpy(retStr,"SLAVE");
            ret=true;
        }
    }
    else if(!strcmp(token,"GET"))
    {
        token = strtok(NULL, " ");
        if(!strcmp(token,"ADR"))
        {
            addr = nRF24L01_get_address();
            sprintf(retStr,"%02x",addr);
            ret=true;
        }
        else if(!strcmp(token,"VER"))
        {
            strcpy(retStr,version);
            ret=true;
        }
        else if(!strcmp(token,"STATE"))
        {
            strcpy(retStr,_master?"MASTER":"SLAVE");
            ret=true;
        }
        else if(!strcmp(token,"ECHO"))
        {
            strcpy(retStr,_echo?"ON":"OFF");
            ret=true;
        }
    }
    else if(!strcmp(token,"HELP"))
    {
        strcpy(retStr,"SET:GET - ADR.VER.MASTER.SLAVE.STATE.ECHO");
        ret=true;
    }

    return ret;
}
