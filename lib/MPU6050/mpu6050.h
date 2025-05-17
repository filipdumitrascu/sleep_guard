#ifndef MPU6050_H
#define MPU6050_H

#include <MPU6050.h>

#define MPU6050_ADDRESS 0x68

extern MPU6050 mpu;

extern float accelDeviation;
extern float gyroDeviation;

/**
 * @brief Initializes the mpu
 */
void mpuInit();

/**
 * @brief Read accelerometer and gyroscope data
 */
void mpuReadData();

#endif
