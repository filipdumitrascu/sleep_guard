#ifndef SLEEP_H
#define SLEEP_H

#include <stdint.h>

struct UserData {
    float accelX{}, accelY{}, accelZ{};   // accelerometer (m/s^2)
    float gyroX{}, gyroY{}, gyroZ{};      // gyroscop (rad/s)
    float bodyTemp{};                     // body temperature (°C)
    float noise{};                        // noise (dB)
    float ambientTemp{};                  // room temperature (°C)
    float humidity{};                     // humidity (%)
    unsigned long timestamp{};            // timestamp (ms)
};
extern UserData sleepData;

enum SleepType {UNDEFINED, DEEP_SLEEP, REM_SLEEP, LIGHT_SLEEP, RESTLESS};
extern SleepType currentSleepType;

// coeficient to calculate average between last 2 measurements
extern const float alpha;

// sleep type countdown
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
 * 
 */
void updateSleepType();

#endif
