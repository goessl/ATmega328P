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



all: adc_main.hex esc.o pid2.o servo_main.hex \
	spi_main.hex spi2.o twi_main.hex twi2.o uart_main.hex uart2_main.hex



#adc
adc_main.hex: adc_main.elf
	$(HC) $(HFLAGS) adc_main.elf adc_main.hex

adc_main.elf: adc_main.o adc.o uart.o
	$(CC) $(LFLAGS) -o adc_main.elf adc_main.o adc.o uart.o
	$(SIZE) adc_main.elf

adc_main.o: adc_main.c $(INC)/adc.h $(INC)/uart.h
	$(CC) $(CFLAGS) -c -o adc_main.o adc_main.c

adc.o: $(SRC)/adc.c $(INC)/adc.h
	$(CC) $(CFLAGS) -c -o adc.o $(SRC)/adc.c

#esc
esc.o: $(SRC)/esc.c $(INC)/esc.h $(INC)/servo.h
	$(CC) $(CFLAGS) -c -o esc.o $(SRC)/esc.c

#pid2
pid2.o: $(SRC)/pid2.c $(INC)/pid2.h
	$(CC) $(CFLAGS) -c -o pid2.o $(SRC)/pid2.c

#ring
ring.o: $(SRC)/ring.c $(INC)/ring.h
	$(CC) $(CFLAGS) -c -o ring.o $(SRC)/ring.c

#servo
servo_main.hex: servo_main.elf
	$(HC) $(HFLAGS) servo_main.elf servo_main.hex

servo_main.elf: servo_main.o servo.o
	$(CC) $(LFLAGS) -o servo_main.elf servo_main.o servo.o
	$(SIZE) servo_main.elf

servo_main.o: servo_main.c $(INC)/servo.h
	$(CC) $(CFLAGS) -c -o servo_main.o servo_main.c

servo.o: $(SRC)/servo.c $(INC)/servo.h
	$(CC) $(CFLAGS) -c -o servo.o $(SRC)/servo.c

#spi
spi_main.hex: spi_main.elf
	$(HC) $(HFLAGS) spi_main.elf spi_main.hex

spi_main.elf: spi_main.o spi.o uart.o
	$(CC) $(LFLAGS) -o spi_main.elf spi_main.o spi.o uart.o
	$(SIZE) spi_main.elf

spi_main.o: spi_main.c $(INC)/spi.h $(INC)/uart.h
	$(CC) $(CFLAGS) -c -o spi_main.o spi_main.c

spi.o: $(SRC)/spi.c $(INC)/spi.h
	$(CC) $(CFLAGS) -c -o spi.o $(SRC)/spi.c

#spi2
spi2.o: $(SRC)/spi2.c $(INC)/spi2.h
	$(CC) $(CFLAGS) -c -o spi2.o $(SRC)/spi2.c

#twi
twi_main.hex: twi_main.elf
	$(HC) $(HFLAGS) twi_main.elf twi_main.hex

twi_main.elf: twi_main.o twi.o uart.o
	$(CC) $(LFLAGS) -o twi_main.elf twi_main.o twi.o uart.o
	$(SIZE) twi_main.elf

twi_main.o: twi_main.c $(INC)/twi.h $(INC)/uart.h
	$(CC) $(CFLAGS) -c -o twi_main.o twi_main.c

twi.o: $(SRC)/twi.c $(INC)/twi.h
	$(CC) $(CFLAGS) -c -o twi.o $(SRC)/twi.c

#twi2
twi2.o: $(SRC)/twi2.c $(INC)/twi2.h
	$(CC) $(CFLAGS) -c -o twi2.o $(SRC)/twi2.c

#uart
uart_main.hex: uart_main.elf
	$(HC) $(HFLAGS) uart_main.elf uart_main.hex

uart_main.elf: uart_main.o uart.o
	$(CC) $(LFLAGS) -o uart_main.elf uart_main.o uart.o
	$(SIZE) uart_main.elf

uart_main.o: uart_main.c $(INC)/uart.h
	$(CC) $(CFLAGS) -c -o uart_main.o uart_main.c

uart.o: $(SRC)/uart.c $(INC)/uart.h
	$(CC) $(CFLAGS) -c -o uart.o $(SRC)/uart.c

#uart2
uart2_main.hex: uart2_main.elf
	$(HC) $(HFLAGS) uart2_main.elf uart2_main.hex

uart2_main.elf: uart2_main.o uart2.o ring.o
	$(CC) $(LFLAGS) -o uart2_main.elf uart2_main.o uart2.o ring.o
	$(SIZE) uart2_main.elf

uart2_main.o: uart2_main.c $(INC)/uart2.h
	$(CC) $(CFLAGS) -c -o uart2_main.o uart2_main.c

uart2.o: $(SRC)/uart2.c $(INC)/uart2.h $(INC)/ring.h
	$(CC) $(CFLAGS) -c -o uart2.o $(SRC)/uart2.c



#Flashing
upload_adc: adc_main.hex
	$(PROG) -Uflash:w:"adc_main.hex":i

upload_servo: servo_main.hex
	$(PROG) -Uflash:w:"servo_main.hex":i

upload_spi: spi_main.hex
	$(PROG) -Uflash:w:"spi_main.hex":i

upload_twi: twi_main.hex
	$(PROG) -Uflash:w:"twi_main.hex":i

upload_uart: uart_main.hex
	$(PROG) -Uflash:w:"uart_main.hex":i

upload_uart2: uart2_main.hex
	$(PROG) -Uflash:w:"uart2_main.hex":i



clean_tmp:
	rm -f *.o *.elf

clean:
	rm -f *.o *.elf *.hex
