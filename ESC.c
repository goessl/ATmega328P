/*
 * ESC.c
 *
 * Created: 25.06.2018 19:59:41
 * Author: Gössl
 */



#include <util/delay.h>
#include "PCA9685.h"
#include "ESC.h"



void ESC_init(size_t* indices, size_t n)
{
    PCA9685_init();
    
    PCA9685_setServo(0, 0);
    PCA9685_setServo(1, 0);
    PCA9685_setServo(2, 0);
    PCA9685_setServo(3, 0);
    _delay_ms(4000);
}

void ESC_initThrottle(size_t* indices, size_t n)
{
    PCA9685_init();
    
    ESC_setMotors(indices, n, 1);
    _delay_ms(4000);
    
    ESC_setMotors(indices, n, 0);
    _delay_ms(4000);
}


void ESC_setMotor(size_t index, double percent)
{
    PCA9685_setServo(index, percent);
}

void ESC_setMotors(size_t* indices, size_t n, double percent)
{
    size_t i;
    
    for(i=0; i<n; i++)
        ESC_setMotor(indices[i], percent);
}
