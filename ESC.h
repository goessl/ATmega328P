/*
 * ESC.h
 *
 * Created: 25.06.2018 20:01:17
 * Author: Gössl
 */



#ifndef ESC_H_
#define ESC_H_



#include <stddef.h>



void ESC_init(size_t* indices, size_t n);
void ESC_initThrottle(size_t* indices, size_t n);

void ESC_setMotor(size_t index, double percent);
void ESC_setMotors(size_t* indices, size_t n, double percent);



#endif /* ESC_H_ */
