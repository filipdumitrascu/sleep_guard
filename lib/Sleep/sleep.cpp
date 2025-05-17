#include "sleep.h"
#include "utils.h"
#include "mpu6050.h"

#include <Arduino.h>
#include <Wire.h>

uint64_t stageStartTime{};

uint64_t deepSleepTime{};
uint64_t remSleepTime{};

uint64_t lightSleepTime{};
uint64_t restlessTime{};

const float alpha{ 0.3 };

SleepType detectSleepType(const UserData& data)
{
    Serial.printf("Accel deviation: %.2f\n", accelDeviation);
    Serial.printf("Gyro deviation: %.2f\n", gyroDeviation);
    Serial.printf("Noise level: %.2f\n", data.noise);

    if ((accelDeviation < 1.0 || gyroDeviation < 0.3) && data.noise < 1.0) {
        return DEEP_SLEEP;
    }

    if ((accelDeviation < 2.0 || gyroDeviation < 0.5) && data.noise < 2.0) {
        return REM_SLEEP;
    }

    if ((accelDeviation < 3.0 || gyroDeviation < 1.0) && data.noise < 3.0) {
        return LIGHT_SLEEP;
    }

    return RESTLESS;
}


void updateSleepType()
{
    if (currentDeviceState != MEASURE) {
        return;
    }

    unsigned long now = millis();
    unsigned long delta = millis() - stageStartTime;

    switch (currentSleepType) {
        case DEEP_SLEEP:
            deepSleepTime += delta;
            break;

        case REM_SLEEP:
            remSleepTime += delta;
            break;

        case LIGHT_SLEEP:
            lightSleepTime += delta;
            break;

        case RESTLESS:
            restlessTime += delta;
            break;

        default:
            break;
    }

    currentSleepType = detectSleepType(sleepData);

    Serial.println(currentSleepType);
    Serial.println();
    delay(300);
}
