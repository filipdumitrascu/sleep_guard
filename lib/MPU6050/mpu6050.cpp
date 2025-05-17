#include "mpu6050.h"
#include "utils.h"
#include "sleep.h"
#include "lcd.h"

MPU6050 mpu;

float accelDeviation{};
float gyroDeviation{};

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

    // Body temperature
    int16_t tempRaw = mpu.getTemperature();
    float bodyTemp = (tempRaw / 340.0) + 36.53;

    // Check movement
    accelDeviation = sqrt(pow(ax_mps2 - sleepData.accelX, 2) +
                          pow(ay_mps2- sleepData.accelY, 2) +
                          pow(az_mps2 - sleepData.accelZ, 2));

    gyroDeviation = sqrt(pow(gx_rps- sleepData.gyroX, 2) +
                         pow(gy_rps - sleepData.gyroY, 2) +
                         pow(gz_rps - sleepData.gyroZ, 2));

    // Update stored data
    sleepData.accelY = alpha * ay_mps2 + (1.0 - alpha) * sleepData.accelY;
    sleepData.accelZ = alpha * az_mps2 + (1.0 - alpha) * sleepData.accelZ;

    sleepData.gyroX = alpha * gx_rps + (1.0 - alpha) * sleepData.gyroX;
    sleepData.gyroY = alpha * gy_rps + (1.0 - alpha) * sleepData.gyroY;
    
    sleepData.gyroZ = alpha * gz_rps + (1.0 - alpha) * sleepData.gyroZ;
    sleepData.bodyTemp = alpha * bodyTemp + (1.0 - alpha) * sleepData.bodyTemp;
}
