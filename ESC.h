/*
 * ESC.h
 *
 * Created: 25.06.2018 20:01:17
 * Author: Gössl
 */



#ifndef ESC_H_
#define ESC_H_



#include <stddef.h>
#include <stdint.h>



void ESC_init(uint8_t** DDRs, uint8_t** PORTs, uint8_t* masks, size_t n);
void ESC_initThrottle(uint8_t** DDRs, uint8_t** PORTs, uint8_t* masks, size_t n);

void ESC_setMotor(size_t index, double percent);
void ESC_setMotors(double* percent);
void ESC_setAllMotors(double percent);



#endif /* ESC_H_ */
