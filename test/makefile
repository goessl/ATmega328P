#Device
MCU=atmega328p
#Port
PORT=/dev/ttyACM0


#Compiler & hex extractor
CC=avr-gcc
SYMBOLS=-D F_CPU=16000000UL
CFLAGS=-O1 -mmcu=$(MCU) -I$(INC) $(SYMBOLS) -Wall -Wextra -Wundef -pedantic \
	-funsigned-char -funsigned-bitfields -ffunction-sections -fdata-sections \
	-fpack-struct -fshort-enums
LFLAGS=-mmcu=$(MCU)

HC=avr-objcopy
HFLAGS=-j .text -j .data -O ihex

#Size ouput
SIZE=avr-size -C --mcu=$(MCU)

#Programmer
PROG=avrdude -P"$(PORT)" -p$(MCU) -carduino -b115200

#File locations
INC=../inc
SRC=../src



all: ADC_main.hex Servo_main.hex SPI_main.hex TWI_main.hex UART_main.hex \
	UART2_main.hex



#ADC
ADC_main.hex: ADC_main.elf
	$(HC) $(HFLAGS) ADC_main.elf ADC_main.hex

ADC_main.elf: ADC_main.o ADC.o UART.o
	$(CC) $(LFLAGS) -o ADC_main.elf ADC_main.o ADC.o UART.o
	$(SIZE) ADC_main.elf

ADC_main.o: ADC_main.c $(INC)/ADC.h $(INC)/UART.h
	$(CC) $(CFLAGS) -c -o ADC_main.o ADC_main.c

ADC.o: $(SRC)/ADC.c $(INC)/ADC.h
	$(CC) $(CFLAGS) -c -o ADC.o $(SRC)/ADC.c

#ESC
ESC.o: $(SRC)/ESC.c $(INC)/ESC.h $(INC)/Servo.h
	$(CC) $(CFLAGS) -c -o ESC.o $(SRC)/ESC.c

#NRF24L01
NRF24L01.o: $(SRC)/NRF24L01.c $(INC)/NRF24L01.h $(INC)/SPI.h
	$(CC) $(CFLAGS) -c -o NRF24L01.o $(SRC)/NRF24L01.c

#NRF24L01_Interrupt
NRF24L01_Interrupt.o: $(SRC)/NRF24L01_Interrupt.c $(INC)/NRF24L01_Interrupt.h \
	$(INC)/SPI.h
	$(CC) $(CFLAGS) -c -o NRF24L01_Interrupt.o $(SRC)/NRF24L01_Interrupt.c

#PID2
PID2.o: $(SRC)/PID2.c $(INC)/PID2.h
	$(CC) $(CFLAGS) -c -o PID2.o $(SRC)/PID2.c

#RING
RING.o: $(SRC)/RING.c $(INC)/RING.h
	$(CC) $(CFLAGS) -c -o RING.o $(SRC)/RING.c

#Servo
Servo_main.hex: Servo_main.elf
	$(HC) $(HFLAGS) Servo_main.elf Servo_main.hex

Servo_main.elf: Servo_main.o Servo.o
	$(CC) $(LFLAGS) -o Servo_main.elf Servo_main.o Servo.o
	$(SIZE) Servo_main.elf

Servo_main.o: Servo_main.c $(INC)/Servo.h
	$(CC) $(CFLAGS) -c -o Servo_main.o Servo_main.c

Servo.o: $(SRC)/Servo.c $(INC)/Servo.h
	$(CC) $(CFLAGS) -c -o Servo.o $(SRC)/Servo.c

#SPI
SPI_main.hex: SPI_main.elf
	$(HC) $(HFLAGS) SPI_main.elf SPI_main.hex

SPI_main.elf: SPI_main.o SPI.o UART.o
	$(CC) $(LFLAGS) -o SPI_main.elf SPI_main.o SPI.o UART.o
	$(SIZE) SPI_main.elf

SPI_main.o: SPI_main.c $(INC)/SPI.h $(INC)/UART.h
	$(CC) $(CFLAGS) -c -o SPI_main.o SPI_main.c

SPI.o: $(SRC)/SPI.c $(INC)/SPI.h
	$(CC) $(CFLAGS) -c -o SPI.o $(SRC)/SPI.c

#SPI2
SPI2.o: $(SRC)/SPI2.c $(INC)/SPI2.h
	$(CC) $(CFLAGS) -c -o SPI2.o $(SRC)/SPI2.c

#TWI
TWI_main.hex: TWI_main.elf
	$(HC) $(HFLAGS) TWI_main.elf TWI_main.hex

TWI_main.elf: TWI_main.o TWI.o UART.o
	$(CC) $(LFLAGS) -o TWI_main.elf TWI_main.o TWI.o UART.o
	$(SIZE) TWI_main.elf

TWI_main.o: TWI_main.c $(INC)/TWI.h $(INC)/UART.h
	$(CC) $(CFLAGS) -c -o TWI_main.o TWI_main.c

TWI.o: $(SRC)/TWI.c $(INC)/TWI.h
	$(CC) $(CFLAGS) -c -o TWI.o $(SRC)/TWI.c

#TWI2
TWI2.o: $(SRC)/TWI2.c $(INC)/TWI2.h
	$(CC) $(CFLAGS) -c -o TWI2.o $(SRC)/TWI2.c

#UART
UART_main.hex: UART_main.elf
	$(HC) $(HFLAGS) UART_main.elf UART_main.hex

UART_main.elf: UART_main.o UART.o
	$(CC) $(LFLAGS) -o UART_main.elf UART_main.o UART.o
	$(SIZE) UART_main.elf

UART_main.o: UART_main.c $(INC)/UART.h
	$(CC) $(CFLAGS) -c -o UART_main.o UART_main.c

UART.o: $(SRC)/UART.c $(INC)/UART.h
	$(CC) $(CFLAGS) -c -o UART.o $(SRC)/UART.c

#UART2
UART2_main.hex: UART2_main.elf
	$(HC) $(HFLAGS) UART2_main.elf UART2_main.hex

UART2_main.elf: UART2_main.o UART2.o RING.o
	$(CC) $(LFLAGS) -o UART2_main.elf UART2_main.o UART2.o RING.o
	$(SIZE) UART2_main.elf

UART2_main.o: UART2_main.c $(INC)/UART2.h
	$(CC) $(CFLAGS) -c -o UART2_main.o UART2_main.c

UART2.o: $(SRC)/UART2.c $(INC)/UART2.h $(INC)/RING.h
	$(CC) $(CFLAGS) -c -o UART2.o $(SRC)/UART2.c



#Flashing
upload_ADC: ADC_main.hex
	$(PROG) -Uflash:w:"ADC_main.hex":i

upload_Servo: Servo_main.hex
	$(PROG) -Uflash:w:"Servo_main.hex":i

upload_SPI: SPI_main.hex
	$(PROG) -Uflash:w:"SPI_main.hex":i

upload_TWI: TWI_main.hex
	$(PROG) -Uflash:w:"TWI_main.hex":i

upload_UART: UART_main.hex
	$(PROG) -Uflash:w:"UART_main.hex":i

upload_UART2: UART2_main.hex
	$(PROG) -Uflash:w:"UART2_main.hex":i



clean_tmp:
	rm -f *.o *.elf

clean:
	rm -f *.o *.elf *.hex
