#Device
MCU=atmega328p
#Port
PORT=/dev/ttyACM0


#Compiler
CC=avr-gcc
SYMBOLS=-D F_CPU=16000000UL
CFLAGS=-O1 -mmcu=$(MCU) -I"$(INC)" $(SYMBOLS) -Wall -Wextra -Wundef -pedantic \
	-funsigned-char -funsigned-bitfields -ffunction-sections -fdata-sections \
	-fpack-struct -fshort-enums
LFLAGS=-mmcu=$(MCU)

#Hex generator
HC=avr-objcopy
HFLAGS=-j .text -j .data -O ihex

#Print size
SIZE=avr-size -C --mcu=$(MCU)

#Programmer
PROG=avrdude -P"$(PORT)" -p$(MCU) -carduino -b115200

#File locations
INC=../inc
SRC=../src

#Files
SOURCES=$(wildcard $(SRC)/*.c)
SRCOBJS=$(patsubst ../src/%,%,$(SOURCES:.c=.o))

TESTS=$(wildcard *.c)
TESTHEXS=$(TESTS:.c=.hex)



#Make all tests AND all sources for demonstration and compile testing
all: $(TESTHEXS) $(SRCOBJS)



#Hex files
%.hex: %.elf
	$(HC) $(HFLAGS) $< $@

%.elf: %.o $(SRCOBJS)
	$(CC) $(LFLAGS) -o $@ $< $(SRCOBJS)
	$(SIZE) $@

#Test objects
%_main.o: %_main.c
	$(CC) $(CFLAGS) -c -o $@ $<

#Source objects
%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<



#Flashing
upload_adc: adc_test.hex
	$(PROG) -Uflash:w:"adc_test.hex":i

upload_servo: servo_test.hex
	$(PROG) -Uflash:w:"servo_test.hex":i

upload_spi: spi_test.hex
	$(PROG) -Uflash:w:"spi_test.hex":i

upload_twi: twi_test.hex
	$(PROG) -Uflash:w:"twi_test.hex":i

upload_uart: uart_test.hex
	$(PROG) -Uflash:w:"uart_test.hex":i

upload_uartint: uartint_test.hex
	$(PROG) -Uflash:w:"uartint_test.hex":i



#Cleaning
.PHONY: clean
clean:
	rm -f *.o *.elf *.hex
