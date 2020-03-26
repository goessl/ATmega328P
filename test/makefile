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
upload_ADC: ADC_main.hex
	$(PROG) -Uflash:w:"ADC_main.hex":i

upload_SERVO: SERVO_main.hex
	$(PROG) -Uflash:w:"SERVO_main.hex":i

upload_SPI: SPI_main.hex
	$(PROG) -Uflash:w:"SPI_main.hex":i

upload_TWI: TWI_main.hex
	$(PROG) -Uflash:w:"TWI_main.hex":i

upload_UART: UART_main.hex
	$(PROG) -Uflash:w:"UART_main.hex":i

upload_UART2: UART2_main.hex
	$(PROG) -Uflash:w:"UART2_main.hex":i



#Cleaning
.PHONY: clean
clean:
	rm -f *.o *.elf *.hex
