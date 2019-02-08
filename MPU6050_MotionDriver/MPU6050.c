/*
 * MPU6050.c
 * 
 * Author:      Sebastian Gössl
 * Hardware:    ATmega328P, MPU6050
 * 
 * LICENSE:
 * MIT License
 * 
 * Copyright (c) 2018 Sebastian Gössl
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */



#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <math.h>
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "MPU6050System.h"
#include "MPU6050.h"



#if MPU6050_INTERRUPT == 0
    #define MPU6050_ISR INT0_vect
#elif MPU6050_INTERRUPT == 1
    #define MPU6050_ISR INT1_vect
#else
    #error "No valid MPU6050_INTERRUPT selected!"
#endif



static inline unsigned short MPU6050_inv_row_2_scale(const signed char *row)
{
    unsigned short b;
    
    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    
    
    return b;
}
static inline unsigned short MPU6050_inv_orientation_matrix_to_scalar(const signed char *mtx)
{
    unsigned short scalar;
    
    scalar = MPU6050_inv_row_2_scale(mtx);
    scalar |= MPU6050_inv_row_2_scale(mtx + 3) << 3;
    scalar |= MPU6050_inv_row_2_scale(mtx + 6) << 6;
    
    
    return scalar;
}
//https://teksab.wordpress.com/2014/11/27/mpu6050-part-1/
static signed char gyro_orientation[9] = {   1, 0, 0,
                                             0, 1, 0,
                                             0, 0, 1};



volatile int MPU6050_interruptFlag = 0;



int MPU6050_init(void)
{
    MPU6050System_init();
    
    #if MPU6050_INTERRUPT == 0
        EICRA |= (1 << ISC01) | (1 << ISC00);
        EIMSK |= (1 << INT0);
    #elif MPU6050_INTERRUPT == 1
        EICRA |= (1 << ISC11) | (1 << ISC10);
        EIMSK |= (1 << INT1);
    #endif
    
    
    if(mpu_init(NULL))
        return 1;
    if(mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))
        return 1;
    if(mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
        return 1;
    if(mpu_set_sample_rate(MPU6050_SAMPLE_RATE))
        return 1;
    
    if(dmp_load_motion_driver_firmware())
        return 1;
    if(dmp_set_orientation(MPU6050_inv_orientation_matrix_to_scalar(gyro_orientation)))
        return 1;
    if(dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_TAP))
        return 1;
    if(dmp_set_fifo_rate(MPU6050_SAMPLE_RATE))
        return 1;
    if(mpu_set_dmp_state(1))
        return 1;
    
    
    return 0;
}


int MPU6050_dataAvailable(void)
{
    if(MPU6050_interruptFlag)
    {
        MPU6050_interruptFlag = 0;
        return 1;
    }
    else
        return 0;
}


void MPU6050_readAllRaw(int16_t* g, int16_t* a, int32_t* q, unsigned long* timestamp, uint8_t* more)
{
    short sensors;
    
    dmp_read_fifo((short*)g, (short*)a, (long*)q, timestamp, &sensors, more);
}

void MPU6050_readAllScaled(double* g, double* a, double* q, unsigned long* timestamp, uint8_t* more)
{
    int16_t gRaw[3], aRaw[3];
    int32_t qRaw[4];
    
    
    
    MPU6050_readAllRaw(gRaw, aRaw, qRaw, timestamp, more);
    
    
    g[0] = gRaw[0] * (2.0 * MPU6050_GYRO_FSR / 0xFFFF);
    g[1] = gRaw[1] * (2.0 * MPU6050_GYRO_FSR / 0xFFFF);
    g[2] = gRaw[2] * (2.0 * MPU6050_GYRO_FSR / 0xFFFF);
    
    a[0] = aRaw[0] * (2.0 * MPU6050_ACCEL_FSR / 0xFFFF);
    a[1] = aRaw[1] * (2.0 * MPU6050_ACCEL_FSR / 0xFFFF);
    a[2] = aRaw[2] * (2.0 * MPU6050_ACCEL_FSR / 0xFFFF);
    
    q[0] = qRaw[0] * (2.0 * MPU6050_GYRO_FSR / 0xFFFF / 1000.0);
    q[1] = qRaw[1] * (2.0 * MPU6050_GYRO_FSR / 0xFFFF / 1000.0);
    q[2] = qRaw[2] * (2.0 * MPU6050_GYRO_FSR / 0xFFFF / 1000.0);
    q[3] = qRaw[3] * (2.0 * MPU6050_GYRO_FSR / 0xFFFF / 1000.0);
}


void MPU6050_quaternionToGravity(double* q, double* g)
{
    g[0] = 2 * (q[1]*q[3] - q[0]*q[2]);
    g[1] = 2 * (q[0]*q[1] + q[2]*q[3]);
    g[2] = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];
}

void MPU6050_quaternionToEuler(double* q, double* e)
{
    //https://www.i2cdevlib.com/forums/topic/24-roll-and-pitch-angles-ranges/?do=findComment&comment=1138
    double g[3];
    
    
    
    MPU6050_quaternionToGravity(q, g);
    
    //p & r need to check for >90 rotation and correct for gravity polarity reversal
    if(g[2] > 0)
    {
        //yaw: (about Z axis)
        e[2] = atan2(2*q[1]*q[2] - 2*q[0]*q[3], 2*q[0]*q[0] + 2*q[1]*q[1] - 1);
        //pitch: (nose up/down, about Y axis)
        //adding tiny fraction of x2 or y2  avoids gimbal lock at cost of slight accuracy
        e[1] = atan2(g[0], sqrt(0.001*g[1]*g[1] + g[2]*g[2]));
        //roll: (tilt left/right, about X axis)
        e[0] = atan2(g[1], sqrt(0.001*g[0]*g[0] + g[2]*g[2]));
    }
    else
    {
        //yaw: (about Z axis)
        e[2] = atan2(2*q[1]*q[2] - 2*q[0]*q[3], 2*q[0]*q[0] + 2*q[1]*q[1] - 1);
        //pitch: (nose up/down, about Y axis)
        //adding tiny fraction of x2 or y2  avoids gimbal lock at cost of slight accuracy
        e[1] = atan2(g[0], -1.0 * sqrt(0.001*g[1]*g[1] + g[2]*g[2]));
        //roll: (tilt left/right, about X axis)
        e[0] = atan2(g[1], -1.0 * sqrt(0.001*g[0]*g[0] + g[2]*g[2]));
    }
}


void MPU6050_readEuler(double* e, unsigned long* timestamp, uint8_t* more)
{
    double g[3], a[3], q[4];
    
    MPU6050_readAllScaled(g, a, q, timestamp, more);
    MPU6050_quaternionToEuler(q, e);
}




ISR(MPU6050_ISR)
{
    MPU6050_interruptFlag = 1;
}
