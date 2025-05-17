#include "sleep.h"
#include "utils.h"

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
    float accelDeviation = fabs(sqrt(data.accelX * data.accelX +
                                data.accelY * data.accelY +
                                data.accelZ * data.accelZ) - 9.81);

    float gyroTotal = sqrt(data.gyroX * data.gyroX +
                           data.gyroY * data.gyroY +
                           data.gyroZ * data.gyroZ);

    float noiseLevel = data.noise;

    Serial.printf("Accel deviation: %.2f\n", accelDeviation);
    Serial.printf("Gyro total: %.2f\n", gyroTotal);
    Serial.printf("Noise level: %.2f\n", noiseLevel);

    if (accelDeviation < 1.0 || gyroTotal < 0.3) {
        if (noiseLevel < 2.0) {
            return DEEP_SLEEP;
        }

        return REM_SLEEP;
    }

    if ((accelDeviation < 2.0 || gyroTotal < 0.5) && noiseLevel < 4.0) {
        return REM_SLEEP;
    }

    if (accelDeviation < 3.0 || gyroTotal < 1.0) {
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
    stageStartTime = now;
}
