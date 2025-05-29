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
    Serial.printf("Noise level (dB SPL): %.2f\n\n", data.noise);

    if ((accelDeviation < DEEP_ACCEL_THRESHOLD && gyroDeviation < DEEP_GYRO_THRESHOLD)
        && data.noise < DEEP_NOISE_THRESHOLD) {
        return DEEP_SLEEP;
    }

    if ((accelDeviation < REM_ACCEL_THRESHOLD && gyroDeviation < REM_GYRO_THRESHOLD)
        && data.noise < REM_NOISE_THRESHOLD) {
        return REM_SLEEP;
    }

    if ((accelDeviation < LIGHT_ACCEL_THRESHOLD && gyroDeviation < LIGHT_GYRO_THRESHOLD)
        && data.noise < LIGHT_NOISE_THRESHOLD) {
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
    unsigned long delta = now - stageStartTime;

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

const char* getSleepMessage() {
    uint64_t maxTime = deepSleepTime;
    SleepType dominant = DEEP_SLEEP;

    if (remSleepTime > maxTime) {
        maxTime = remSleepTime;
        dominant = REM_SLEEP;
    }
    if (lightSleepTime > maxTime) {
        maxTime = lightSleepTime;
        dominant = LIGHT_SLEEP;
    }
    if (restlessTime > maxTime) {
        dominant = RESTLESS;
    }

    switch (dominant) {
        case DEEP_SLEEP:
            return "You had a deep sleep, which is excellent for physical \\
            recovery and memory consolidation. Keep maintaining a consistent \\
            bedtime and avoiding screens before sleep to continue getting \\
            high-quality rest.";

        case REM_SLEEP:
            return "Your sleep was mostly REM, the phase associated with dreaming and emotional processing. This suggests your brain was quite active during the night. Consider incorporating relaxation techniques before bed to balance your sleep cycle.";

        case LIGHT_SLEEP:
            return "Your sleep was mostly light, which may not be as restorative. This could be due to stress or an inconsistent sleep schedule. Try improving your sleep hygiene by limiting caffeine and establishing a relaxing bedtime routine.";

        case RESTLESS:
            return "Your sleep was mostly restless, which can affect both physical and mental recovery. It may help to evaluate your sleep environment—ensure it's cool, dark, and quiet—and consider stress-reduction practices like meditation or journaling before bed.";

        default:
            return "Sleep data is unavailable. Make sure your device is worn correctly and synced to capture your sleep patterns accurately.";
    }
}
