#Device
MCU=atmega328p
#Port
PORT=/dev/ttyACM0


#Compiler & hex extractor
CC=avr-gcc
SYMBOLS=-D F_CPU=16000000UL -D UART_STD
CFLAGS=-O1 -mmcu=$(MCU) -I$(INC) $(SYMBOLS) -Wall -Wextra -Wundef -pedantic -funsigned-char -funsigned-bitfields -ffunction-sections -fdata-sections -fpack-struct -fshort-enums
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



all: UART_main.hex



#ADC
ADC.o: $(SRC)/ADC.c $(INC)/ADC.h
	$(CC) $(CFLAGS) -c -o ADC.o $(SRC)/ADC.c

#ESC
ESC.o: $(SRC)/ESC.c $(INC)/ESC.h $(INC)/Servo.h
	$(CC) $(CFLAGS) -c -o ESC.o $(SRC)/ESC.c

#NRF24L01
NRF24L01.o: $(SRC)/NRF24L01.c $(INC)/NRF24L01.h $(INC)/SPI.h
	$(CC) $(CFLAGS) -c -o NRF24L01.o $(SRC)/NRF24L01.c

#NRF24L01_Interrupt
NRF24L01_Interrupt.o: $(SRC)/NRF24L01_Interrupt.c $(INC)/NRF24L01_Interrupt.h $(INC)/SPI.h
	$(CC) $(CFLAGS) -c -o NRF24L01_Interrupt.o $(SRC)/NRF24L01_Interrupt.c

#PID2
PID2.o: $(SRC)/PID2.c $(INC)/PID2.h
	$(CC) $(CFLAGS) -c -o PID2.o $(SRC)/PID2.c

#RING
RING.o: $(SRC)/RING.c $(INC)/RING.h
	$(CC) $(CFLAGS) -c -o RING.o $(SRC)/RING.c

#Servo
Servo.o: $(SRC)/Servo.c $(INC)/Servo.h
	$(CC) $(CFLAGS) -c -o Servo.o $(SRC)/Servo.c

#SPI
SPI.o: $(SRC)/SPI.c $(INC)/SPI.h
	$(CC) $(CFLAGS) -c -o SPI.o $(SRC)/SPI.c

#SPI2
SPI2.o: $(SRC)/SPI2.c $(INC)/SPI2.h
	$(CC) $(CFLAGS) -c -o SPI2.o $(SRC)/SPI2.c

#TWI
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
UART2.o: $(SRC)/UART2.c $(INC)/UART2.h $(INC)/RING.h
	$(CC) $(CFLAGS) -c -o UART2.o $(SRC)/UART2.c



#Flashing
upload_UART:
	$(PROG) -Uflash:w:"UART_main.hex":i



clean_tmp:
	rm -f *.o *.elf

clean:
	rm -f *.o *.elf *.hex
