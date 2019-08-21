# ATmega328P

Driver library for basic functionalities of the ATmega328P (Arduino UNO) microcontroller.
 * UART - UART (blocking)
 * UART2 - UART (interrupt based) (uses RING.h)
 * SPI - SPI Master
 * SPI2 - SPI Master (interrupt based)
 * TWI - I2C Master
 * ADC - Analog to digital converter (interrupt based)
 * Servo - Servo driver (interrupt based)
 * ESC - ESC driver (uses Servo.h)
 * PID - PID controller driver (interrupt based)
 * PID2 - PID controller driver with variable frequency (accessible iterate function) (interrupt based)
 * RING - Ring buffer implementation
 * NRF24L01 - NRF24L01 library (blocking) (uses SPI)
 * NRF24L01_Interrupt - NRF24L01 library (interrupt based) (uses SPI)

## Getting Started

### Prerequisites

This project is written in C and nothing except the standard libraries (avr-libc) are needed.

### Installation

Download this repository and save it somewhere where you can find it later.
Add the repository folder to your include paths and the source files (.c) to your project when compiling.

#### Atmel Studio 7.0

When you created a new project right-click on the project (in the solution explorer) and go to Properties->Toolchain->[Your Compiler]->Directories and add a new directory.
Select this repository in the file chooser and click ok. This lets the compiler find the header files when you include them in your sources.
You also have to add the needed source files to your project (if you use UART.h then you also need to add UART.c). You can add them by right-clicking on the project, go to "Add" and then press "Existing item...".
Now select the source files (.c) and press "Add". (You can also add them as link so it doesn't always copies the whole file to your project directory by pressing "Add as link"
which appears when you click the small downwards facing arrow to the right of the "Add" button in the file chooser)

## Usage

The functions of every module start with the module name in all capitals, followed by an underline and then the function name in camel case formatting. E.g.
```
UART_transmitBurst()
```

Most modules have an init function that needs to be called for every other function to work properly. E.g.
```
UART_init()
```

Also make sure to enable interrupts before calling any function of modules that use interrupts!

## License (MIT)

MIT License

Copyright (c) 2019 Sebastian Gössl

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
