/*
 * servo.h
 * 
 * Interrupt based servo driver.
 * 
 * Author:      Sebastian Goessl
 * Hardware:    ATmega328P
 * 
 * LICENSE:
 * MIT License
 * 
 * Copyright (c) 2018 Sebastian Goessl
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */



#ifndef SERVO_H_
#define SERVO_H_



#include <stddef.h> //size_t type
#include <stdint.h> //uint8_t type



//default to timer 1
#ifndef SERVO_TIMER
    #define SERVO_TIMER 1
#endif

/** Period in microseconds of the generated PWM signals. */
#define SERVO_BASE_US   (20 * 1000)
/** Minimum duty cycle in microseconds. */
#define SERVO_MIN_US    (1 * 1000)
/** Maximum duty cycle in microseconds. */
#define SERVO_MAX_US    (2 * 1000)
/** Maximum number of registered servos. */
#define SERVO_MAX_N     (SERVO_BASE_US / SERVO_MAX_US)



/**
 * Configures the given pins as outputs and registers up to SERVO_MAX_N
 * servos.
 * For every registered servo a PWM signal (period: SERVO_BASE_US,
 * minimum duty cycle: SERVO_MIN_US, maximum duty cycle: SERVO_MAX_US,
 * all in micro seconds) is generated on the provided PORTs.
 * All servos are set to 0 (SERVO_MIN_US).
 * Interrupts have to be enabled.
 * 
 * @param DDRs array with pointers to the DDR registers of the PORT registers
 * the servos are attached to
 * @param PORTs array with pointers to the PORT registers
 * the servos are attached to
 * @param masks bit masks with the bits, corresponding to the bits
 * in the PORT registers the servos are attached to, enabled
 * @param n number of servos
 */
void servo_init(uint8_t **DDRs, uint8_t **PORTs, uint8_t *maks, size_t n);

/**
 * Sets the value of the servo with the given index.
 * 0 for minimum (SERVO_MIN_US), 0xFF for maximum (SERVO_MAX_US) duty cycle.
 * 
 * @param index index of the servo to set
 * @param value value to set the servo to between 0 and 0xFF
 */
void servo_setServo(size_t index, uint8_t value);
/**
 * Sets the value of the servo with the given index.
 * 0 for minimum (SERVO_MIN_US), 1 for maximum (SERVO_MAX_US) duty cycle.
 * 
 * @param index index of the servo to set
 * @param value value to set the servo to between 0 and 1
 */
void servo_setServoScaled(size_t index, double percent);
/**
 * Sets the values of all servos.
 * 0 for minimum (SERVO_MIN_US), 0xFF for maximum (SERVO_MAX_US) duty cycle.
 * 
 * @param values location with the values between 0 and 0xFF
 * to set the servos to
 */
void servo_setServos(uint8_t *values);
/**
 * Sets the values of all servos.
 * 0 for minimum (SERVO_MIN_US), 1 for maximum (SERVO_MAX_US) duty cycle.
 * 
 * @param values location with the values between 0 and 1
 * to set the servos to
 */
void servo_setServosScaled(double *percents);
/**
 * Sets the values of all servos to the same value.
 * 0 for minimum (SERVO_MIN_US), 0xFF for maximum (SERVO_MAX_US) duty cycle.
 * 
 * @param value value to set all servos to between 0 and 0xFF
 */
void servo_setAllServos(uint8_t value);
/**
 * Sets the values of all servos to the same value.
 * 0 for minimum (SERVO_MIN_US), 1 for maximum (SERVO_MAX_US) duty cycle.
 * 
 * @param value value to set all servos to between 0 and 1
 */
void servo_setAllServosScaled(double percent);



#endif /* SERVO_H_ */
