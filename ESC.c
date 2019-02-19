/*
 * ESC.c
 *
 * Created: 25.06.2018 19:59:41
 * Author: Gössl
 */



#include <util/delay.h>
#include "Servo.h"
#include "ESC.h"



void ESC_init(uint8_t** DDRs, uint8_t** PORTs, uint8_t* masks, size_t n)
{
    SERVO_init(DDRs, PORTs, masks, n);
    
    SERVO_setAllServos(0);
    _delay_ms(4000);
}

void ESC_initThrottle(uint8_t** DDRs, uint8_t** PORTs, uint8_t* masks, size_t n)
{
    SERVO_init(DDRs, PORTs, masks, n);
    
    SERVO_setAllServos(1);
    _delay_ms(4000);
    
    SERVO_setAllServos(0);
    _delay_ms(4000);
}



void ESC_setMotor(size_t index, double percent)
{
    SERVO_setServo(index, percent);
}

void ESC_setMotors(double* percent)
{
    SERVO_setServos(percent);
}

void ESC_setAllMotors(double percent)
{
    SERVO_setAllServos(percent);
}
