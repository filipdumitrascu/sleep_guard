#include "mpu6050.h"
#include "utils.h"
#include "sleep.h"
#include "lcd.h"
#include <math.h>

MPU6050 mpu;

float accelDeviation{};
float gyroDeviation{};

// Function to calculate the z-axis angle in degrees
float calculateZAngle(float ax, float ay, float az) {
    float magnitude = sqrt(ax * ax + ay * ay + az * az);
    return acos(az / magnitude) * (180.0 / M_PI);
}

void mpuInit() {
    mpu.initialize();

    while (!mpu.testConnection()) {
        Serial.println("MPU6050 connection failed");
        lcdPrintMessage("Initializing", "mpu6050", 0);
        delay(100);
    }

    Serial.println("MPU6050 connection successful");
    lcdPrintMessage("Mpu6050", "initialized!", 2000);
}

void mpuReadData() {
    if (currentDeviceState != MEASURE) {
        return;
    }

    int16_t ax_raw, ay_raw, az_raw, gx_raw, gy_raw, gz_raw;
    mpu.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);

    // Convert raw acceleration to m/s^2
    float ax = (ax_raw / 16384.0) * 9.81;
    float ay = (ay_raw / 16384.0) * 9.81;
    float az = (az_raw / 16384.0) * 9.81;

    // Calculate z-axis angle
    float zAngle = calculateZAngle(ax, ay, az);

    // Implement rolling median and variance calculations here
    // Store zAngle values over time to analyze movement patterns
    // Identify periods of low variance as potential sleep intervals

    // Convert raw gyroscope data to rad/s
    float gx = (gx_raw / 131.0) * (M_PI / 180.0);
    float gy = (gy_raw / 131.0) * (M_PI / 180.0);
    float gz = (gz_raw / 131.0) * (M_PI / 180.0);

    // Read temperature from MPU6050
    int16_t tempRaw = mpu.getTemperature();
    float bodyTemp = (tempRaw / 340.0) + 36.53;

    // Calculate deviations
    accelDeviation = sqrt(pow(ax - sleepData.accelX, 2) +
                          pow(ay - sleepData.accelY, 2) +
                          pow(az - sleepData.accelZ, 2));

    gyroDeviation = sqrt(pow(gx - sleepData.gyroX, 2) +
                         pow(gy - sleepData.gyroY, 2) +
                         pow(gz - sleepData.gyroZ, 2));

    // Update stored data with exponential smoothing
    sleepData.accelY = alpha * ay + (1.0 - alpha) * sleepData.accelY;
    sleepData.accelZ = alpha * az + (1.0 - alpha) * sleepData.accelZ;

    sleepData.gyroX = alpha * gx + (1.0 - alpha) * sleepData.gyroX;
    sleepData.gyroY = alpha * gy + (1.0 - alpha) * sleepData.gyroY;
    sleepData.gyroZ = alpha * gz + (1.0 - alpha) * sleepData.gyroZ;
    sleepData.bodyTemp = alpha * bodyTemp + (1.0 - alpha) * sleepData.bodyTemp;
}
