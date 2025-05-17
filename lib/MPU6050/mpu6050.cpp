#include "mpu6050.h"
#include "utils.h"
#include "lcd.h"

MPU6050 mpu;

void mpuInit()
{
    mpu.initialize();

    while (!mpu.testConnection()) {
        Serial.println("MPU6050 connection failed");
        delay(100);
    }

    Serial.println("MPU6050 connection successful");
}

void mpuReadData()
{
    if (currentDeviceState != MEASURE) {
        return;
    }

    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // Acceleration(m/s^2)
    float ax_mps2 = (ax / 16384.0) * 9.81;
    float ay_mps2 = (ay / 16384.0) * 9.81;
    float az_mps2 = (az / 16384.0) * 9.81;

    // Gyroscop(rad/s)
    float gx_rps = (gx / 131.0) * (M_PI / 180.0);
    float gy_rps = (gy / 131.0) * (M_PI / 180.0);
    float gz_rps = (gz / 131.0) * (M_PI / 180.0);

    int16_t tempRaw = mpu.getTemperature();
    float tempC = (tempRaw / 340.0) + 36.53;
}
