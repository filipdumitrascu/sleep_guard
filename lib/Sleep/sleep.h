#ifndef SLEEP_H
#define SLEEP_H

#include <stdint.h>

// Tunable thresholds for classifying sleep stages
#define DEEP_ACCEL_THRESHOLD     0.8f
#define REM_ACCEL_THRESHOLD      1.5f
#define LIGHT_ACCEL_THRESHOLD    3.0f

#define DEEP_GYRO_THRESHOLD      0.25f
#define REM_GYRO_THRESHOLD       0.5f
#define LIGHT_GYRO_THRESHOLD     1.0f

#define DEEP_NOISE_THRESHOLD     120.0f
#define REM_NOISE_THRESHOLD      122.5f
#define LIGHT_NOISE_THRESHOLD    125.0f

struct UserData {
    float accelX{}, accelY{}, accelZ{};   // accelerometer (m/s^2)
    float gyroX{}, gyroY{}, gyroZ{};      // gyroscop (rad/s)
    float bodyTemp{};                     // body temperature (°C)
    float noise{};                        // noise (dB)
    float ambientTemp{};                  // room temperature (°C)
    float humidity{};                     // humidity (%)
};
extern UserData sleepData;

enum SleepType {UNDEFINED, DEEP_SLEEP, REM_SLEEP, LIGHT_SLEEP, RESTLESS};
extern SleepType currentSleepType;

// coeficient to calculate average between last 2 measurements
extern const float alpha;

// sleep type countdown
extern uint64_t stageStartTime;
extern uint64_t deepSleepTime;

extern uint64_t remSleepTime;
extern uint64_t lightSleepTime;

extern uint64_t restlessTime;

/**
 * @brief Returns the current sleep type.
 * 
 * @param data 
 * @return SleepType
 */
SleepType detectSleepType(const UserData& data);

/**
 * @brief Updates the current sleep type.
 */
void updateSleepType();

/**
 * @brief Sleep advices.
 * 
 */
const char* getSleepMessage();

#endif
