/*
 * esc.h
 * 
 * Generic ESC driver.
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



#ifndef ESC_H_
#define ESC_H_



#include <stddef.h> //size_t type
#include <stdint.h> //uint8_t type
#include "servo.h"  //servo symbols: SERVO_MAX_N, ...



/**
 * Configures the given pins as outputs and registers up to SERVO_MAX_N
 * ESCs, initializing them (takes 4 seconds).
 * For every registered ESC a PWM signal (period: SERVO_BASE_US,
 * minimum duty cycle: SERVO_MIN_US, maximum duty cycle: SERVO_MAX_US,
 * all in micro seconds) is generated on the provided PORTs.
 * All ESCs are set to 0 (SERVO_MIN_US).
 * Interrupts have to be enabled.
 * 
 * @param DDRs array with pointers to the DDR registers of the PORT registers
 * the ESCs are attached to
 * @param PORTs array with pointers to the PORT registers
 * the ESCs are attached to
 * @param masks bit masks with the bits, corresponding to the bits
 * in the PORT registers the ESCs are attached to, enabled
 * @param n number of ESCs
 */
void esc_init(uint8_t **DDRs, uint8_t **PORTs, uint8_t *masks, size_t n);
/**
 * Configures the given pins as outputs and registers up to SERVO_MAX_N
 * ESCs, initializing them and setting throttle thresholds (takes 8 seconds).
 * For every registered ESC a PWM signal (period: SERVO_BASE_US,
 * minimum duty cycle: SERVO_MIN_US, maximum duty cycle: SERVO_MAX_US,
 * all in micro seconds) is generated on the provided PORTs.
 * All ESCs are set to 0 (SERVO_MIN_US).
 * Interrupts have to be enabled.
 * 
 * @param DDRs array with pointers to the DDR registers of the PORT registers
 * the ESCs are attached to
 * @param PORTs array with pointers to the PORT registers
 * the ESCs are attached to
 * @param masks bit masks with the bits, corresponding to the bits
 * in the PORT registers the ESCs are attached to, enabled
 * @param n number of ESCs
 */
void esc_initThrottle(uint8_t **DDRs, uint8_t **PORTs, uint8_t *masks, size_t n);

/**
 * Sets the throttle of the ESC with the given index.
 * 0 for minimum (SERVO_MIN_US), 0xFF for maximum (SERVO_MAX_US) throttle.
 * 
 * @param index index of the ESC to set
 * @param value throttle as value between 0 and 0xFF to set the ESC to
 */
void esc_setMotor(size_t index, uint8_t value);
/**
 * Sets the throttle of the ESC with the given index.
 * 0 for minimum (SERVO_MIN_US), 1 for maximum (SERVO_MAX_US) throttle.
 * 
 * @param index index of the ESC to set
 * @param value throttle as value between 0 and 1 to set the ESC to
 */
void esc_setMotorScaled(size_t index, double percent);
/**
 * Sets the throttle of all ESCs.
 * 0 for minimum (SERVO_MIN_US), 0xFF for maximum (SERVO_MAX_US) throttle.
 * 
 * @param values location with the throttles as values between 0 and 0xFF
 * to set the ESCs to
 */
void esc_setMotors(uint8_t *values);
/**
 * Sets the throttle of all ESCs.
 * 0 for minimum (SERVO_MIN_US), 1 for maximum (SERVO_MAX_US) throttle.
 * 
 * @param values location with the throttles as values between 0 and 1
 * to set the ESCs to
 */
void esc_setMotorsScaled(double *percents);
/**
 * Sets the throttle of all ESCs to the same value.
 * 0 for minimum (SERVO_MIN_US), 0xFF for maximum (SERVO_MAX_US) throttle.
 * 
 * @param value throttle as value between 0 and 0xFF to set all ESCs to
 */
void esc_setAllMotors(uint8_t value);
/**
 * Sets the throttle of all ESCs to the same value.
 * 0 for minimum (SERVO_MIN_US), 1 for maximum (SERVO_MAX_US) throttle.
 * 
 * @param value throttle as value between 0 and 1 to set all ESCs to
 */
void esc_setAllMotorsScaled(double percent);



#endif /* ESC_H_ */
